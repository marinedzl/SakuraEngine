#include "stdafx.h"
#include "Shader.h"

Shader::Pass::Pass()
	: VS(nullptr)
	, PS(nullptr)
	, BlendState(nullptr)
	, CullMode(D3D11_CULL_BACK)
	, DepthStencilState(nullptr)
	, propertyCount(0)
	, properties(nullptr)
{

}

Shader::Pass::~Pass()
{
	SAFE_RELEASE(VS);
	SAFE_RELEASE(PS);
	SAFE_DELETE(properties);
}

Shader::Shader()
{
}

Shader::~Shader()
{
}
