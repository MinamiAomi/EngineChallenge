cbuffer WorldTransform : register(b0)
{
    matrix world;       // ���[���h�s��
}

cbuffer Camera : register(b1)
{
    matrix view;        // �r���[�s��
    matrix  proj;       // �ˉe�s��
    float cameraPos;    // �J�������W
}

cbuffer Material : register(b2)
{
    float3 ambient;	// �����i�A���r�G���g�j
    float3 deffuse;	// �g�U���i�f�B�t���[�Y�j
    float3 specular;	// ���ʔ��ˌ��i�X�y�L�����[�j
    float alpha;		// �����x�i�A���t�@�j
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