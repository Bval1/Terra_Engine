// No textures
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
    float4 materialColor;
    float specularIntensity;
    float specularPower;
    float padding[2];
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{

    const float3 vToL = lightPos - worldPos;  // vector to light
    const float distToL = length(vToL);     // magnitude of above
    const float dirToL = vToL / distToL;    // normalized vector to light
	
    // diffuse attentuation
    const float att = 1.0f / (attConstant + attLinear * distToL + attQuadratic * (distToL * distToL)); // Hadarmard product (member by member multiplication)
    
    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

    // reflected light vector(reflect instrintic avaible in HLSL)
    const float3 w = n * dot(vToL, n); // reflection of the vector going from light to pixel
    const float3 r = w * 2.0f - vToL; // reflect it on the surface of the pixel using the normal of

    
    // calculate specular intensity based on angle between viewing vector	
	// compare that reflection to the vector from the camera to the pixel to see 
	// how much its pointing in the direction of the camera using the dot product with pow
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity *
    pow( max( 0.0f, dot(normalize(-r), normalize(worldPos)) ), specularPower);
   
    const float3 color = { materialColor.r, materialColor.g, materialColor.b };
    return float4(saturate((diffuse + ambient) * color + specular), 1.0f); // final color
}

//HLSL intrinsticfunctions:
//https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-intrinsic-functions