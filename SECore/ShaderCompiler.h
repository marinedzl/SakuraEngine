#pragma once
#include "Shader.h"

class ShaderCompiler
{
public:
	ShaderCompiler();
	virtual ~ShaderCompiler();
	void Release();
	bool Init();
	bool LoadShader(Shader* shader, const char* filename);
private:
	bool ParsePass(Shader* shader, const Json::Value& root, const std::string& path);
private:
	bool StringToBlendOp(const std::string& str, D3D11_BLEND_OP& dst);
	bool String2Blend(const std::string& str, D3D11_BLEND& dst);
	bool StringToCullMode(const std::string& str, D3D11_CULL_MODE& dst);
	bool StringToZWrite(const std::string& str, BOOL& dst);
	bool StringToZTest(const std::string& str, D3D11_COMPARISON_FUNC& dst);
	bool String2AlphaTest(const std::string& str, AlphaTestMode& dst);
	bool String2Proprety(const std::string& str, Shader::Property::Type& dst);
private:
	typedef std::map<std::string, D3D11_BLEND_OP> BlendOpMap;
	typedef std::map<std::string, D3D11_BLEND> BlendMap;
	typedef std::map<std::string, D3D11_COMPARISON_FUNC> ZTestMap;
	typedef std::map<std::string, AlphaTestMode> AlphaTestMap;
	typedef std::map<std::string, Shader::Property::Type> PropertyMap;
private:
	BlendOpMap mBlendOpMap;
	BlendMap mBlendMap;
	ZTestMap mZTestMap;
	AlphaTestMap mAlphaTestMap;
	PropertyMap mPropertyMap;
};

extern ShaderCompiler gShaderCompiler;
