#include "stdafx.h"
#include "Core.h"
#include "ConstantBufferManager.h"
#include "Texture.h"
#include "MeshShader.h"

namespace
{
	Shader::PropertyType GetPropertyType(const char* name)
	{
		if (strcmp(name, "DiffColor") == 0)
			return Shader::eColor;
		else if (strcmp(name, "DiffTex") == 0)
			return Shader::eTexture;
		else if (strcmp(name, "CutOff") == 0)
			return Shader::eFloat;
		else
			return Shader::eInvalid;
	}
}

MeshShader::MeshShader()
	: mMainTexture(nullptr)
	, mPS(nullptr)
{

}

MeshShader::~MeshShader()
{
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mMainTexture);
}

bool MeshShader::Init()
{
	bool ret = false;
	ID3D11Device* device = gCore.GetDevice();
	buffer file;
	CHECK(LoadBinaryFile(file, "AlphaTest-Diffuse.cso"));
	CHECK(SUCCEEDED(device->CreatePixelShader(file.ptr(), file.size(), nullptr, &mPS)));

	mBuffer.color = Color(1, 1, 1, 1);
	mBuffer.cutoff = 0.5f;

	ret = true;
Exit0:
	return ret;
}

bool MeshShader::SetValue(const char* name, const void* data)
{
	bool ret = false;
	Shader::PropertyType type = GetPropertyType(name);
	switch (type)
	{
	case Shader::eFloat:
		mBuffer.cutoff = *(float*)data;
		break;
	case Shader::eColor:
		mBuffer.color = *(Color*)data;
		break;
	default:
		goto Exit0;
	}
	ret = true;
Exit0:
	return ret;
}

bool MeshShader::SetTexture(const char* name, ITexture* texture)
{
	if (GetPropertyType(name) == Shader::eTexture)
	{
		SAFE_RELEASE(mMainTexture);
		mMainTexture = dynamic_cast<Texture*>(texture);
		if (mMainTexture)
			mMainTexture->AddRef();
	}
	else
	{
		return false;
	}
	return true;
}

Shader::PropertyType MeshShader::GetPropertyType(const char * name) const
{
	return ::GetPropertyType(name);
}

bool MeshShader::SetPass(size_t passIndex) const
{
	bool ret = false;
	ID3D11DeviceContext* context = gCore.GetContext();
	CHECK(context);

	// commit buffer
	ConstantBuffer* cb = gConstantBufferManager.GetBuffer(eCBProperty);
	if (cb)
	{
		ConstantBuffer* cb = gConstantBufferManager.GetBuffer(eCBProperty);
		cb->SetData(0, &mBuffer, sizeof(mBuffer));
		cb->Commit();
	}

	// set texture
	if (mMainTexture)
		mMainTexture->SetSlot(texture_slot_reserve + 0);

	context->PSSetShader(mPS, nullptr, 0);

	ret = true;
Exit0:
	return ret;
}
