#include <common.hlsli>

cbuffer CBModel : register(b1)
{
	matrix MATRIX_M;
};

struct V2P
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

V2P main(Vertex IN)
{
	V2P o;
	o.pos = mul(mul(IN.pos, MATRIX_M), MATRIX_VP);
	o.uv = IN.uv0;
	return o;
}
