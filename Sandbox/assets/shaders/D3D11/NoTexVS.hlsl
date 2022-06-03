// No textures

cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};


struct VSOut
{
    float3 cameraPos : Position;    // relative to camera
    float3 normal : Normal;
    float4 pos : SV_Position;
};


VSOut main(float3 pos : Position, float3 n : Normal)
{
    VSOut vso;

    vso.cameraPos = (float3) mul(float4(pos, 1.0f), modelView);

    // don't translate normal only rotate, therefore exclude 4th row and column
    vso.normal = mul(n, (float3x3)modelView);
    
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    
    return vso;
}