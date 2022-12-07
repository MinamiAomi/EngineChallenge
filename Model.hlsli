cbuffer WorldTransform : register(b0)
{
    matrix c_world;       // ワールド行列
}

cbuffer Camera : register(b1)
{
    matrix c_view;        // ビュー行列
    matrix  c_proj;       // 射影行列
    float3 c_cameraPos;    // カメラ座標
}

cbuffer Material : register(b2)
{
    float3 c_ambient : packoffset(c0); // 環境光（アンビエント）
    float3 c_diffuse : packoffset(c1); // 拡散光（ディフューズ）
    float c_shininess : packoffset(c1.w); // 光沢度
    float3 c_specular : packoffset(c2); // 鏡面反射光（スペキュラー）
    float c_alpha : packoffset(c2.w); // 透明度（アルファ）
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
    float4 worldpos : POSITION;
    float3 normal : NORMAL;     // 法線
    float2 uv : TEXCOORD;       //　uv座標
};