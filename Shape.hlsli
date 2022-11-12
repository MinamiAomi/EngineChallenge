cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; // 座標変換行列
}

struct VSOutput
{
    float4 svpos : SV_Position;
    float4 color : COLOR;
};