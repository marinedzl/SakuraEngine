#include "stdafx.h"
#include "Core.h"
#include "ImageData.h"
#include "Texture.h"

Texture::Texture()
	: mPtr(nullptr)
	, mSRV(nullptr)
	, mSampler(nullptr)
	, mWidth(0)
	, mHeight(0)
{
}

Texture::~Texture()
{
	SAFE_RELEASE(mPtr);
	SAFE_RELEASE(mSRV);
	SAFE_RELEASE(mSampler);
}

bool Texture::Create(UINT w, UINT h, const void* bits)
{
	bool ret = false;

	mWidth = w;
	mHeight = h;

	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = w;
	textureDesc.Height = h;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (bits)
	{
		D3D11_SUBRESOURCE_DATA subres;
		subres.pSysMem = bits;
		subres.SysMemPitch = w * 4;
		subres.SysMemSlicePitch = 0; // Not needed since this is a 2d texture

		CHECK(SUCCEEDED(device->CreateTexture2D(&textureDesc, &subres, &mPtr)));
	}
	else
	{
		CHECK(SUCCEEDED(device->CreateTexture2D(&textureDesc, nullptr, &mPtr)));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
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

	ret = true;
Exit0:
	return ret;
}

bool Texture::LoadFromFile(const char* filename)
{
	bool ret = false;
	ImageData imageData;

	CHECK(filename);
	CHECK(imageData.LoadFromFile(filename));

	mWidth = imageData.GetWidth();
	mHeight = imageData.GetHegiht();

	ret = Create(mWidth, mHeight, imageData.GetBits());

	ret = true;
Exit0:
	return ret;
}

void Texture::SetSlot(int slot)
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		context->PSSetShaderResources(slot, 1, &mSRV);
		context->PSSetSamplers(slot, 1, &mSampler);
	}
}
