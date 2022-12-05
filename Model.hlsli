cbuffer Transform : register(b0)
{
    matrix viewproj;    // カメラ行列
    matrix world;       // ワールド行列
}

struct VSInput
{
    float4 pos : POSITION;      // 座標
    float3 normal : NORMAL;     // 法線
    float2 uv : TEXCOORD;          //　uv座標
};

struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用座標
    float3 normal : NORMAL;     // 法線
    float2 uv : TEXCOORD;       //　uv座標
};