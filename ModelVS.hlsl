#include "Model.hlsli"

VSOutput main(VSInput input)
{

    float4 worldNormal = normalize(mul(c_world, float4(input.normal, 0)));
    float4 worldPos = mul(c_world, input.pos);
    
    VSOutput output;
    output.svpos = mul(mul(mul(c_proj, c_view), c_world), input.pos);
    output.worldpos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = input.uv;
    return output;
}