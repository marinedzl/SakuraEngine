#include <common.hlsli>

struct Light
{
	float3 color;
	float pad;
	float3 position;
	float pad2;
	float3 att;
	float pad3;
};

cbuffer CBLight : register(b1) { Light light; };

DecalTexture(_GBuffer, 0);

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

float4 main(float4 screenPos : SV_POSITION) : SV_TARGET
{
	float2 uv = screenPos.xy / SCREEN_SIZE;
	float4 dlColor = tex2D(_GBuffer, uv);
	float3 N = dlColor.rgb * 2.0f - 1.0f;
	float depth = dlColor.w;
	
	if(depth == 0)
		return float4(0, 0, 0, 0);
	
	N.b = 1.0f - sqrt(N.r * N.r + N.g * N.g);
	
	float4 screenPosition;
	screenPosition.x = 2.0f * uv.x - 1.0f;
	screenPosition.y = 1.0f - 2.0f * uv.y;
	screenPosition.z = depth;
	screenPosition.w = 1.0f;
	
	float4 pos = mul(screenPosition, INV_VP);
	pos.xyz = pos.xyz / pos.w;

	float3 L = normalize(light.position - pos.xyz); // light direction
	float3 V = normalize(EYE_POS - pos.xyz); // view direction

	float LdotN = max(0, dot(L, N));

	float3 diffuse = light.color * LdotN;

	float d = distance(light.position, pos.xyz);
	float att = 1.0 / (light.att[0] + d * light.att[1] + d * d * light.att[2]);

	float3 color = att * diffuse;
	
	return float4(color.rgb, 1.0f);
}
