#include "Model.hlsli"

Texture2D<float4> t_texture : register(t0);
SamplerState s_sampler : register(s0);

struct directionLight
{
    float3 color;
    float3 direction;
};

float4 main(VSOutput input) : SV_TARGET
{
    directionLight light;
    light.color = float3(1.0f, 1.0f, 1.0f);
    light.direction = normalize(float3(1.0f, -1.0f, 1.0f));
    
    float3 ambientColor = float3(0.2f, 0.2f, 0.2f);
    // テクスチャの色
    float4 textureColor = t_texture.Sample(s_sampler, input.uv);
    // シェーディングによる色
    float4 shadeColor = float4(ambientColor * c_ambient, c_alpha);
    
    //// 拡散反射光 ↓
    //// ピクセルの法線とライトの方向の内積を計算する
    float dot_light_normal = saturate(dot(-light.direction, input.normal));
    //// 拡散反射光
    float3 diffuse = dot_light_normal * float3(1.0f, 1.0f, 1.0f);
    
    // 鏡面反射光 ↓
    // 頂点から視点へのベクトル
    float3 toEye = normalize(c_cameraPos - input.worldpos.xyz);
    // ライトベクトルの面との反射ベクトル
    float3 refVec = normalize(light.direction + 2 * dot_light_normal * input.normal);
    // 反射ベクトルと視点へのベクトルの内積
    float dot_refVec_toEye = saturate(dot(refVec, toEye));
   
    // 鏡面反射の強さを絞る
    dot_refVec_toEye = pow(dot_refVec_toEye, c_shininess);
    // 鏡面反射光
    float3 specular = dot_refVec_toEye * c_specular;
    
    shadeColor.rgb += (diffuse + specular) * light.color;
      
    return shadeColor * textureColor;
}