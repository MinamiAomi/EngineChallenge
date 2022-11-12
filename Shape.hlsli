cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; // À•W•ÏŠ·s—ñ
}

struct VSOutput
{
    float4 svpos : SV_Position;
    float4 color : COLOR;
};