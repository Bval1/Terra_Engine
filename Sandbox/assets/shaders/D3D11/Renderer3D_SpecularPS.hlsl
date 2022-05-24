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

Texture2D tex;
Texture2D spec;

SamplerState splr;


float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_TARGET
{
    // fragment to light vector data
    const float3 vToL = lightPos - worldPos; 
    const float3 distToL = length(vToL); 
    const float dirToL = vToL / distToL; 

    // attenuation
    const float att = 1.0f / (attConstant + attLinear * distToL + attQuadratic * (distToL * distToL));

    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

    // reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;

    
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float4 specularSample = spec.Sample(splr, tc);
    const float3 specularReflectionColor = specularSample.rgb;
    const float specularPower = pow(2.0f, specularSample.a * 12.0f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * 
    pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);

 
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularReflectionColor), 1.0f);
}