#pragma once
#include "TAnyValue.h"

enum ValueType
{
	eNull,
	eInteger,
	eFloat,
	eString,
	ePtr,
	eVector3,
	eVector4,
	eColor,
	eTexture2D,
};

template<typename T> struct TypeTraits { static ValueType GetType(); };
template<> struct TypeTraits<int> { static ValueType GetType() { return eInteger; } };
template<> struct TypeTraits<float> { static ValueType GetType() { return eFloat; } };
template<> struct TypeTraits<std::string> { static ValueType GetType() { return eString; } };
template<> struct TypeTraits<void*> { static ValueType GetType() { return ePtr; } };
template<> struct TypeTraits<Color> { static ValueType GetType() { return eColor; } };
template<> struct TypeTraits<Vector3> { static ValueType GetType() { return eVector3; } };
template<> struct TypeTraits<Vector4> { static ValueType GetType() { return eVector4; } };
template<> struct TypeTraits<Texture*> { static ValueType GetType() { return eTexture2D; } };

typedef TAnyValue<ValueType> AnyValue;
extern TAnyValue<ValueType>::ValueAccesser* ValueAccessers[];

ValueType String2ValueType(const std::string& type);

bool String2Value(const Json::Value& json, ValueType type, AnyValue& value);
