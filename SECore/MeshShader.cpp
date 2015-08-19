#include "stdafx.h"
#include "Core.h"
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
	, mCBPS(nullptr)
	, isDirty(false)
{

}

MeshShader::~MeshShader()
{
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mCBPS);
	SAFE_RELEASE(mMainTexture);
}

bool MeshShader::Init()
{
	bool ret = false;
	ID3D11Device* device = gCore.GetDevice();
	buffer file;
	CHECK(LoadBinaryFile(file, "AlphaTest-Diffuse.cso"));
	CHECK(SUCCEEDED(device->CreatePixelShader(file.ptr(), file.size(), nullptr, &mPS)));

	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(buffDesc));
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.ByteWidth = sizeof(mPSBuffer);
	CHECK(SUCCEEDED(device->CreateBuffer(&buffDesc, nullptr, &mCBPS)));

	mPSBuffer.color = Color(1, 1, 1, 1);
	mPSBuffer.cutoff = 0.5f;
	isDirty = true;

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
		mPSBuffer.cutoff = *(float*)data;
		break;
	case Shader::eColor:
		mPSBuffer.color = *(Color*)data;
		break;
	default:
		goto Exit0;
	}
	isDirty = true;
	ret = true;
Exit0:
	return ret;
}

bool MeshShader::SetTexture(const char* name, SECore::Texture* texture)
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

	if (isDirty)
	{
		// commit buffer
		D3D11_MAPPED_SUBRESOURCE mr;
		if (SUCCEEDED(context->Map(mCBPS, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
		{
			memcpy(mr.pData, &mPSBuffer, sizeof(CBPS));
			context->Unmap(mCBPS, 0);
		}
	}

	context->PSSetConstantBuffers(2, 1, &mCBPS);

	// set texture
	if (mMainTexture)
		mMainTexture->SetSlot(0);

	context->PSSetShader(mPS, nullptr, 0);

	ret = true;
Exit0:
	return ret;
}
