#include <common.hlsli>

cbuffer CBSkinned : register(b1)
{
	matrix MATRIX_M;
	matrix MATRIX_M_SKIN[256];
};

struct V2P
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
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
	m = mul(m, MATRIX_VP);
	o.position = mul(IN.pos, m);
	o.depthPosition = o.position;
	return o;
}
