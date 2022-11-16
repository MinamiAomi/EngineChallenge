cbuffer CommonConstBufferData : register(b0)
{
    matrix camera; // カメラ座標変換行列
}

cbuffer ConstBufferData : register(b1)
{
    float4 color; // 色
    matrix world; // ワールド座標変換行列
}

struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD;       // uv値
};