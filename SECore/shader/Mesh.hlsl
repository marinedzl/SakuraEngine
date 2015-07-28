#include <common.hlsli>

Input main(appdata_t IN)
{
	Input o;
	o.pos = MVP(IN.pos);
	o.uv = IN.uv0;
	return o;
}
