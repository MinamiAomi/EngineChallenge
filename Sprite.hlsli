cbuffer CommonConstBufferData : register(b0)
{
    matrix camera; // �J�������W�ϊ��s��
}

cbuffer ConstBufferData : register(b1)
{
    float4 color; // �F
    matrix world; // ���[���h���W�ϊ��s��
}

struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD;       // uv�l
};