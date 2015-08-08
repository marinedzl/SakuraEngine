#include <common.hlsli>

Input main(appdata_t IN)
{
	Input o;
	float weight = 1 - IN.weights[0] - IN.weights[1] - IN.weights[2];
	matrix m;
	m = MATRIX_M_SKIN[IN.boneIndices[0]] * IN.weights[0]
		+ MATRIX_M_SKIN[IN.boneIndices[1]] * IN.weights[1]
		+ MATRIX_M_SKIN[IN.boneIndices[2]] * IN.weights[2]
		+ MATRIX_M_SKIN[IN.boneIndices[3]] * weight;
	m = mul(m, MATRIX_M);
	m = mul(m, MATRIX_VP);
	o.pos = mul(IN.pos, m);
	o.uv = IN.uv0;
	return o;
}
