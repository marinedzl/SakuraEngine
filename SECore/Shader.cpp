#include "stdafx.h"
#include "Material.h"
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

bool Shader::Pass::InitMaterial(Material* material) const
{
	if (!mProperties.empty())
	{
		Properties::const_iterator iter = mProperties.begin();
		Properties::const_iterator iterEnd = mProperties.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second->GetValue().GetType() == eTexture2D)
				continue;
			material->SetValue(iter->first.c_str(), iter->second->GetValue());
		}
	}
	return true;
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
	DeleteList(mPasses);
}

bool Shader::LoadFromFile(const char* filename)
{
	return gShaderCompiler.LoadShader(this, filename);
}

const Shader::Property * Shader::GetProperty(const char * name) const
{
	const Shader::Property * ret = nullptr;
	for (size_t i = 0; i < mPasses.size(); i++)
	{
		ret = mPasses[i]->GetProperty(name);
		if (ret)
			break;
	}
	return ret;
}
