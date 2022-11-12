
cbuffer ConstBufferDataMaterial : register(b0)
{
    float4 color; // F(RGBA)
}

cbuffer ConstBufferDataTransform : register(b1)
{
    matrix mat; // À•W•ÏŠ·s—ñ
}

struct VSOutput
{
    float4 svpos : SV_Position;
    float2 uv : TEXCOORD;
};