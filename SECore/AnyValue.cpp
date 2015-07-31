#include "stdafx.h"
#include "AnyValue.h"

template<typename T>
struct TValueAccesser : TAnyValue<ValueType>::ValueAccesser
{
	void Alloc(TAnyValue<ValueType>& value) { value.data = new T(); }
	void Assign(TAnyValue<ValueType>& value, const void* from) { *(T*)value.data = *(T*)from; }
	void Free(TAnyValue<ValueType>& value) { delete ((T*)value.data); }
	size_t GetSize() { return sizeof(T); }
};

TValueAccesser<int> IntegerAccesser;
TValueAccesser<float> FloatAccesser;
TValueAccesser<std::string> StringAccesser;
TValueAccesser<void*> PtrAccesser;
TValueAccesser<Vector3> Vector3Accesser;
TValueAccesser<Vector4> Vector4Accesser;
TValueAccesser<Color> ColorAccesser;
TValueAccesser<Texture*> TextureAccesser;

TAnyValue<ValueType>::ValueAccesser* ValueAccessers[] = {
	nullptr,
	&IntegerAccesser,
	&FloatAccesser,
	&StringAccesser,
	&PtrAccesser,
	&Vector3Accesser,
	&Vector4Accesser,
	&ColorAccesser,
	&TextureAccesser,
};

DWORD ConverColor2Dword(const Color& color)
{
	static const float factor = 255.0f;
	int a = (int)(color.a * factor);
	int b = (int)(color.b * factor);
	int g = (int)(color.g * factor);
	int r = (int)(color.r * factor);
	return r << 24 | g << 16 | b << 8 | a;
}

Color ConvertDwordToColor(DWORD dw)
{
	static const float factor = 1 / 255.0f;
	int a = dw & 255;
	int b = dw >> 8 & 255;
	int g = dw >> 16 & 255;
	int r = dw >> 24 & 255;
	return Color(r * factor, g * factor, b * factor, a * factor);
}

bool String2Value(const Json::Value& json, ValueType type, AnyValue& value)
{
	bool ret = false;
	switch (type)
	{
	case eInteger:
		value = json.asInt();
		break;
	case eFloat:
		value = (float)json.asDouble();
		break;
	case eColor:
	{
		const char* str = json.asCString();
		DWORD dwColor = strtoul(str, NULL, 16);
		Color color = ConvertDwordToColor(dwColor);
		value = color;
	}
		break;
	default:
		CHECK(false);
		break;
	}
	ret = true;
Exit0:
	return ret;
}
