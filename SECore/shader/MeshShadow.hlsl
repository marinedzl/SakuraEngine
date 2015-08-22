#include <common.hlsli>

cbuffer CBModel : register(b1)
{
	matrix MATRIX_M;
};

struct V2P
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

V2P main(Vertex IN)
{
	V2P o;
	o.position = mul(mul(IN.pos, MATRIX_M), MATRIX_VP);
	o.depthPosition = o.position;
	return o;
}
