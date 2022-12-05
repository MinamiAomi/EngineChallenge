cbuffer Transform : register(b0)
{
    matrix viewproj;    // �J�����s��
    matrix world;       // ���[���h�s��
}

struct VSInput
{
    float4 pos : POSITION;      // ���W
    float3 normal : NORMAL;     // �@��
    float2 uv : TEXCOORD;          //�@uv���W
};

struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���W
    float3 normal : NORMAL;     // �@��
    float2 uv : TEXCOORD;       //�@uv���W
};