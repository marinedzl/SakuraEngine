#include <common.hlsli>

Input main(appdata_t IN)
{
	Input o;
	o.pos = IN.pos + float4(IN.norm, 0);
	o.pos = MVP(o.pos);
	o.uv = IN.uv0;
	return o;
}
