#pragma pack_matrix(row_major)

cbuffer CB : register(b0)
{
	matrix MVP;
	float4 _MainColor;
};

struct appdata_t
{
	float4 pos : POSITION;
};

struct V2P
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

V2P main(appdata_t IN)
{
	V2P o;
	o.pos = mul(IN.pos, MVP);
	o.color = _MainColor;
	return o;
}
