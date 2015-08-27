struct V2P
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

float4 main(V2P IN) : SV_TARGET
{
	float depthValue;
	float4 color;
	
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = IN.depthPosition.z / IN.depthPosition.w;
	color = float4(depthValue, depthValue, depthValue, 1.0f);
	
	return color;
}
