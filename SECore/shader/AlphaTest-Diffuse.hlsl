#define surface
#include <common.hlsli>

decl_buffer
{
	float4 _Color;
	float _CutOff;
};

decl_texture(_MainTex, 0);

void surf(Input IN, inout SurfaceOutput o)
{
	float4 c = tex2D(_MainTex, IN.uv) * _Color;
	clip(c.a < _CutOff ? -1 : 1);
	o = (SurfaceOutput)0;
	o.Albedo = c.rgb;
	o.Alpha = c.a;
}
