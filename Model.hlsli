cbuffer WorldTransform : register(b0)
{
    matrix world;       // ワールド行列
}

cbuffer Camera : register(b1)
{
    matrix view;        // ビュー行列
    matrix  proj;       // 射影行列
    float cameraPos;    // カメラ座標
}

cbuffer Material : register(b2)
{
    float3 ambient;	// 環境光（アンビエント）
    float3 deffuse;	// 拡散光（ディフューズ）
    float3 specular;	// 鏡面反射光（スペキュラー）
    float alpha;		// 透明度（アルファ）
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