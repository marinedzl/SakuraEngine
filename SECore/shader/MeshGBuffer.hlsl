#include <common.hlsli>

cbuffer CBModel : register(b1)
{
	matrix MATRIX_M;
};

struct V2P
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float4 depth : TEXCOORD;
};

V2P main(Vertex IN)
{
	V2P o;
	o.pos = mul(mul(IN.pos, MATRIX_M), MATRIX_VP);
	o.normal = mul(IN.normal.xyz, (float3x3)MATRIX_M);
	o.depth = o.pos;
	return o;
}
