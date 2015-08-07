#define surface
#include <common.hlsli>

cbuffer Property : register(b0)
{
	float4 _Color;
	float _CutOff;
};

Sampler2D(_MainTex, 0);

void surf(Input IN, inout SurfaceOutput o)
{
	float4 c = tex2D(_MainTex, IN.uv) * _Color;
	clip(c.a < _CutOff ? -1 : 1);
	o = (SurfaceOutput)0;
	o.Albedo = c.rgb;
	o.Alpha = c.a;
}
