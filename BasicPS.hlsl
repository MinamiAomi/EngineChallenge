
cbuffer ConstBufferData : register(b0)
{
    float4 color; // êF(RGBA)
}

float4 main() : SV_TARGET
{
    return color;
}