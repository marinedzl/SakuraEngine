#include <common.hlsli>

struct appdata_c
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

struct V2F
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

V2F main(appdata_c IN)
{
	V2F o;

	o.pos = IN.pos;
	o.uv = IN.uv;
	o.color = IN.color;

	return o;
}
