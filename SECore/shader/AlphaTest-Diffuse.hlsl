#include <common.hlsli>

cbuffer Property : register(b2)
{
	float4 _Color;
	float _CutOff;
};

Texture2D _MainTex : register(t0);
SamplerState _MainTexS : register(s0);

Texture2D LT : register(t3);
SamplerState LTS : register(s3);

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(V2P IN) : SV_TARGET
{
	float4 c = _MainTex.Sample(_MainTexS, IN.uv) * _Color;
	clip(c.a < _CutOff ? -1 : 1);

	float3 albedo = c.rgb;
	float alpha = c.a;

	float2 uv = IN.pos.xy / SCREEN_SIZE;
	float3 lightColor = LT.Sample(LTS, uv).rgb;

	float3 finalColor = albedo * (AmbientColor + lightColor);

	return float4(finalColor, alpha);
}
