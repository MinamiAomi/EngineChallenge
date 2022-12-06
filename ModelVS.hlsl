#include "Model.hlsli"

VSOutput main(VSInput input)
{

    float4 worldNormal = normalize(mul(world, float4(input.normal, 0)));
    
    VSOutput output;
    output.svpos = mul(mul(mul(proj, view), world), input.pos);
    output.normal = worldNormal.xyz;
    output.uv = input.uv;
    return output;
}