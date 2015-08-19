#pragma pack_matrix(row_major)

struct Vertex
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float2 uv2 : TEXCOORD2;
	uint4 boneIndices : BLENDINDICES;
	float3 weights : BLENDWEIGHT;
};

cbuffer CBGobal : register(b0)
{
	matrix MATRIX_VP;
	matrix INV_VP;
	float3 EYE_POS; float nouse;
	float2 SCREEN_SIZE; float2 nouse2;
	float4 AmbientColor;
};
