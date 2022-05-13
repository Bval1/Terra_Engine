Texture2D tex; // Texture2D tex : register(t0);  to explicitly bind to a slot

SamplerState splr;

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    return tex.Sample(splr, tc); // lookup into the texture using the texcoord
}