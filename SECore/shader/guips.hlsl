#include <common.hlsli>

struct V2F
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

decl_texture(_MainTex, 0);

float4 main(V2F IN) : SV_TARGET
{
	float4 col = tex2D(_MainTex, IN.uv) * IN.color;
	return col;
}
