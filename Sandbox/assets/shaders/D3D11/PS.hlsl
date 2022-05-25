cbuffer Cbuf : register(b1) // bind to slot 1 so it doesn't overwrite light cbuf for the other pixel shaders
{
    float4 solidColor;
};

float4 main() : SV_TARGET
{
    return solidColor;
}