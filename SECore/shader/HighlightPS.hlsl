#define surface
#include <common.hlsli>

decl_buffer
{
	float4 _HighlightColor;
};

void surf(Input IN, inout SurfaceOutput o)
{
	float4 c = _HighlightColor;
	o = (SurfaceOutput)0;
	o.Albedo = c.rgb;
	o.Alpha = c.a;
}
