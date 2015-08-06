#include "stdafx.h"
#include "Material.h"

Material::Material()
	: mShader(nullptr)
{

}

Material::~Material()
{
	SAFE_RELEASE(mShader);
}

void Material::SetColor(const char* name, const Color& value)
{
	CHECK(mShader);
	CHECK(mShader->SetValue(name, &value));
Exit0:
	;
}

void Material::SetTexture(const char* name, SECore::Texture* texture)
{
	CHECK(mShader);
	CHECK(mShader->SetTexture(name, texture));
Exit0:
	;
}

void Material::SetShader(Shader* shader)
{
	SAFE_RELEASE(mShader);
	mShader = shader;
}

void Material::SetFloat(const char * name, float value)
{
	mShader->SetValue(name, &value);
}
