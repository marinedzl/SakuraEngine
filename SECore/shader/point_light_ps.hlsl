#include <common.hlsli>

struct Light
{
	float3 diffuse;
	float diffusePower;

	float3 specular;
	float specularPower;

	float3 position;
	float specularHardness;

	float range;
	float intensity;
	float2 nouse;
};

cbuffer CBLight : register(b1) { Light gLight; };

float computerSpecLight(float3 V, float3 N, float3 L, float shininess)
{
	V = normalize(V);
	N = normalize(N);
	L = normalize(L);
	float specularLight = 0;
	float3 Half_vector = normalize(L + V);
	float HdotN = max(0, dot(Half_vector, N));
	specularLight = pow(HdotN, abs(shininess));

	return specularLight;
}

Texture2D normalDepthTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(float4 screenPos : SV_POSITION) : SV_TARGET
{
	float2 uv = screenPos.xy / SCREEN_SIZE;
	float4 dlColor = normalDepthTexture.Sample(Sampler, uv);
	float3 normal = dlColor.rgb * 2.0f - 1.0f;
	float depth = dlColor.w;
	
	if(depth == 0)
		return float4(0, 0, 0, 0);
	
	normal.b = 1.0f - sqrt(normal.r * normal.r + normal.g * normal.g);
	
	float4 screenPosition;
	screenPosition.x = 2.0f * uv.x - 1.0f;
	screenPosition.y = 1.0f - 2.0f * uv.y;
	screenPosition.z = depth;
	screenPosition.w = 1.0f;
	
	float4 pos = mul(screenPosition, INV_VP);
	pos.xyz = pos.xyz / pos.w;
	
	float3 dir = gLight.position - pos.xyz;
	float distance = length(dir);
	dir = normalize(dir);
	
	float factor = max(0, gLight.range - distance) * max(0, dot(dir, normal)) * gLight.intensity;
	float3 color = 0;
	color = gLight.diffuse * factor;
	
	float shadowmask = 1;
	float spec = computerSpecLight(normalize(EYE_POS.xyz - pos.xyz), normal, dir, dlColor.b) * min(gLight.specularPower, shadowmask);
	color += spec * color.rgb;
	
	return float4(color.rgb, 1.0f);
}
