#include "Sprite.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = mul(mul(camera, world), pos);
    output.uv = uv;
    return output;
}