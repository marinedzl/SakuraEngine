#define surface

struct V2P
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(V2P IN) : SV_TARGET
{
	return IN.color;
}
