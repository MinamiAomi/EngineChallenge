cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; // ���W�ϊ��s��
}

struct VSOutput
{
    float4 svpos : SV_Position;
    float4 color : COLOR;
};