cbuffer CommonConstBufferData : register(b0)
{
    matrix camera; // カメラ座標変換行列
}

cbuffer ConstBufferData : register(b1)
{
    matrix world; // ワールド座標変換行列
}

struct VSOutput
{
    float4 svpos : SV_Position;
    float4 color : COLOR;
};