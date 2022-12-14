
cbuffer ConstBufferDataMaterial : register(b0)
{
    float4 color; // 色(RGBA)
}

cbuffer ConstBufferDataTransform : register(b1)
{
    matrix mat; // 座標変換行列
}

struct VSOutput
{
    float4 svpos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};