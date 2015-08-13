#pragma pack_matrix(row_major)

cbuffer Property : register(b0)
{
	matrix MVP;
};

struct appdata_t
{
	float4 pos : POSITION;
};

struct V2P
{
	float4 pos : SV_POSITION;
};

V2P main(appdata_t IN)
{
	V2P o;
	o.pos = mul(IN.pos, MVP);
	return o;
}
