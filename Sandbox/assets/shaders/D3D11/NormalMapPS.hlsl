//#define USECOLOR
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConstant; // attenuation values: https://wiki.ogre3d.org/-Point+Light+Attenuation
    float attLinear;
    float attQuadratic;
};

cbuffer ObjectCBuf  // set per object for each object rendered, use slot 1
{
#ifdef USECOLOR 
    float4 materialColor;
#endif
    float specularIntensity;
    float specularPower;
    bool hasNormalMap;
    float padding[1];
};

cbuffer TransformCbuf
{
    matrix modelView;
    matrix modelViewProj;
};

Texture2D tex;
Texture2D normalMap : register(t2);     // register 1 reserved for specmap

SamplerState splr;


float4 main(float3 viewPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord) : SV_TARGET
{
    if (hasNormalMap)
    {
        // build the tranform (rotation) into tangent space
        const float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),
            normalize(n)
        );
        const float3 normalSample = normalMap.Sample(splr, tc).xyz;
        n = normalSample * 2.0f - 1.0f; // convert 0.0 to 1.0 range to a range of -1.0 to 1.0
        n.y = -n.y; // negated since hlsl has +y going down y axis
        //n.x = normalSample.x;
        //n.z = normalSample.z * 2.0f - 1.0f;    // -z points toward the camera
        //n.z = normalSample.z;
        //n = mul(n, (float3x3) modelView);       // only 3x3 since you dont want to translate normals, only rotate them
        n = mul(n, tanToView);
    }
    
    // fragment to light vector data
    const float3 vToL = lightPos - viewPos; // vector to light
    const float3 distToL = length(vToL); // magnitude of above
    const float3 dirToL = vToL / distToL; // normalized vector to light

    // attenuation
    const float att = 1.0f / (attConstant + attLinear * distToL + attQuadratic * (distToL * distToL));

    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

    // reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;

    
    // calculate specular intensity based on angle between viewing vector and reflection vector, 
    // narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity *
    pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

    // final color
#ifdef USECOLOR
    const float3 color = { materialColor.r, materialColor.g, materialColor.b };
    return float4(saturate((diffuse + ambient) * color + specular), 1.0f);
#else
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
#endif
    
}