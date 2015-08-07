#pragma pack_matrix(row_major)

#define Sampler2D(name, slot) \
Texture2D name : register(t##[slot]);\
SamplerState name##S : register(s##[slot])

#define tex2D(tex, uv) tex.Sample(tex##S, uv)

#define MVP(pos) mul(mul(pos, MATRIX_M), MATRIX_VP)

struct appdata_t
{
	float4 pos : POSITION;
	float3 norm : NORMAL;
	float4 tangent : TANGENT;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float2 uv2 : TEXCOORD2;
	uint4 boneIndices : BLENDINDICES;
	float3 weights : BLENDWEIGHT;
};

struct Input
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer CBGobal : register(b1)
{
	matrix MATRIX_VP;
};

cbuffer CBModel : register(b2)
{
	matrix MATRIX_M;
};

cbuffer CBSkinned : register(b3)
{
	matrix MATRIX_M_SKIN[256];
};

#ifdef surface
struct SurfaceOutput
{
	float3 Albedo;
	float3 Normal;
	float3 Emission;
	float Specular;
	float Gloss;
	float Alpha;
};
void surf(Input IN, inout SurfaceOutput o);
float4 main(Input IN) : SV_TARGET
{
	SurfaceOutput so;
	surf(IN, so);

	float4 col;
	col.rgb = so.Albedo;
	col.a = so.Alpha;
	return col;
}
#endif
