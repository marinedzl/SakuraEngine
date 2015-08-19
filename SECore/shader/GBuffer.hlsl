struct V2P
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float4 depth : TEXCOORD;
};

float4 main(V2P IN) : SV_TARGET
{
	float3 normal = IN.normal * 0.5f + 0.5f;
	float depth = IN.depth.z / IN.depth.w;
	return float4(normal, depth);
}
