#include <common.hlsli>

cbuffer CBSkinned : register(b1)
{
	matrix MATRIX_M;
	matrix MATRIX_M_SKIN[256];
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
	float weight = 1 - IN.weights[0] - IN.weights[1] - IN.weights[2];
	matrix m;
	m = MATRIX_M_SKIN[IN.boneIndices[0]] * IN.weights[0]
		+ MATRIX_M_SKIN[IN.boneIndices[1]] * IN.weights[1]
		+ MATRIX_M_SKIN[IN.boneIndices[2]] * IN.weights[2]
		+ MATRIX_M_SKIN[IN.boneIndices[3]] * weight;
	m = mul(m, MATRIX_M);
	o.normal = mul(IN.normal.xyz, (float3x3)m);
	m = mul(m, MATRIX_VP);
	o.pos = mul(IN.pos, m);
	o.depth = o.pos;
	return o;
}
