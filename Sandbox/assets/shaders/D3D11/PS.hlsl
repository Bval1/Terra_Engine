cbuffer Cbuf
{
    float4 solidColor;
};

float4 main() : SV_TARGET
{
    return solidColor;
}