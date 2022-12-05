#include "Model.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 ambientColor = { 0.1f, 0.1f, 0.1f };
    
    float3 light = normalize(float3(1, -1, 1));
    float diffuse = saturate(dot(-light, input.normal));
    float3 diffuse3 = float3(diffuse, diffuse, diffuse) + ambientColor;
    //float brightness = diffuse + 0.3f;
    
    float4 texColor = float4(tex.Sample(smp, input.uv));
    
    return float4(texColor.rgb * diffuse3, texColor.a);
    
   // return texColor;

}