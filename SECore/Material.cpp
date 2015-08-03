#include "stdafx.h"
#include "Material.h"

MaterialImpl::MaterialImpl()
	: mShader(nullptr)
{

}

MaterialImpl::~MaterialImpl()
{
	SAFE_RELEASE(mShader);
}

void MaterialImpl::SetColor(const char* name, const Color& value)
{
	CHECK(mShader);
	CHECK(mShader->SetValue(name, &value));
Exit0:
	;
}

void MaterialImpl::SetTexture(const char* name, ITexture* texture)
{
	CHECK(mShader);
	CHECK(mShader->SetTexture(name, texture));
Exit0:
	;
}

void MaterialImpl::SetShader(Shader* shader)
{
	SAFE_RELEASE(mShader);
	mShader = shader;
}

void MaterialImpl::SetFloat(const char * name, float value)
{
	mShader->SetValue(name, &value);
}
