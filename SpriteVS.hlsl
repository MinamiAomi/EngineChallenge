#include "Sprite.hlsli"

VSoutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSoutput output;
    output.svpos = mul(mat, pos);
    output.uv = uv;
    return output;
}