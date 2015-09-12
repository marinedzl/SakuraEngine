#include "stdafx.h"
#include "Core.h"
#include "ImageData.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture()
	: mPtr(nullptr)
	, mSRV(nullptr)
	, mSampler(nullptr)
{
}

RenderTexture::~RenderTexture()
{
	SAFE_RELEASE(mPtr);
	SAFE_RELEASE(mSRV);
	SAFE_RELEASE(mSampler);
}

bool RenderTexture::Create(int w, int h)
{
	bool ret = false;

	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	ZeroMemory(&mDesc, sizeof(mDesc));
	mDesc.Width = w;
	mDesc.Height = h;
	mDesc.MipLevels = 1;
	mDesc.ArraySize = 1;
	mDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	mDesc.SampleDesc.Count = 1;
	mDesc.Usage = D3D11_USAGE_DEFAULT;
	mDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	mDesc.CPUAccessFlags = 0;
	mDesc.MiscFlags = 0;

	CHECK(SUCCEEDED(device->CreateTexture2D(&mDesc, nullptr, &mPtr)));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = mDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	CHECK(SUCCEEDED(device->CreateShaderResourceView(mPtr, &shaderResourceViewDesc, &mSRV)));

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CHECK(SUCCEEDED(device->CreateSamplerState(&samplerDesc, &mSampler)));

	CHECK(__super::Create(mPtr));

	ret = true;
Exit0:
	return ret;
}

void RenderTexture::SetSlot(int slot, bool close)
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		if (close)
		{
			ID3D11ShaderResourceView* t = { nullptr };
			ID3D11SamplerState* s = { nullptr };
			context->PSSetShaderResources(slot, 1, &t);
			context->PSSetSamplers(slot, 1, &s);
		}
		else
		{
			context->PSSetShaderResources(slot, 1, &mSRV);
			context->PSSetSamplers(slot, 1, &mSampler);
		}
	}
}

bool RenderTexture::CaptureToFile(const char * filename)
{
	bool ret = false;
	HRESULT hr;
	ID3D11Texture2D* temp = nullptr;
	ID3D11Device* device = gCore.GetDevice();
	ID3D11DeviceContext* context = gCore.GetContext();

	D3D11_TEXTURE2D_DESC desc;
	mPtr->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = device->CreateTexture2D(&desc, nullptr, &temp);
	CHECK(SUCCEEDED(hr) && "Create texture failed!");

	gCore.GetContext()->CopyResource(temp, mPtr);

	D3D11_MAPPED_SUBRESOURCE md;
	hr = context->Map(temp, 0, D3D11_MAP_READ, 0, &md);
	CHECK(SUCCEEDED(hr));
	SaveTextureToFile(filename, md.pData, mDesc.Width, mDesc.Height);
	context->Unmap(temp, 0);

	ret = true;
Exit0:
	SAFE_RELEASE(temp);
	return ret;
}
