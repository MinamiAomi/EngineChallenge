#include "Shape.hlsli"

VSOutput main(float4 pos : POSITION, float4 color : COLOR)
{
    VSOutput output;
    output.svpos = mul(mul(camera, world), pos);
    output.color = color;
    return output;
}