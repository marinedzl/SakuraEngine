#include "stdafx.h"
#include "ShaderCompiler.h"
#include "Shader.h"

Shader::Pass::Pass()
	: VS(nullptr)
	, PS(nullptr)
	, BlendState(nullptr)
	, CullMode(D3D11_CULL_BACK)
	, DepthStencilState(nullptr)
	, buffSize(0)
	, textureCount(0)
{

}

Shader::Pass::~Pass()
{
	DeleteMap(mProperties);
	SAFE_RELEASE(VS);
	SAFE_RELEASE(PS);
}

const Shader::Property* Shader::Pass::GetProperty(const char* name) const
{
	Properties::const_iterator iter = mProperties.find(name);
	return iter == mProperties.end() ? nullptr : iter->second;
}

Shader::Property* Shader::Pass::AddProperty(const char* name)
{
	Property* prop = new Property();
	mProperties.insert(std::make_pair(name, prop));
	return prop;
}

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::LoadFromFile(const char* filename)
{
	return gShaderCompiler.LoadShader(this, filename);
}
