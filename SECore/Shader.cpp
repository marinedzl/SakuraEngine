#include "stdafx.h"
#include "Shader.h"

Shader::Pass::Pass()
	: VS(nullptr)
	, PS(nullptr)
	, BlendState(nullptr)
	, CullMode(D3D11_CULL_BACK)
	, DepthStencilState(nullptr)
{

}

Shader::Pass::~Pass()
{
	SAFE_RELEASE(VS);
	SAFE_RELEASE(PS);
}

const Shader::Property* Shader::Pass::GetProperty(const char* name) const
{
	Properties::const_iterator iter = mProperties.find(name);
	return iter == mProperties.end() ? nullptr : &iter->second;
}

Shader::Shader()
{
}

Shader::~Shader()
{
}
