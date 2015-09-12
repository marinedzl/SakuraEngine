#include "stdafx.h"
#include "Core.h"
#include "RenderTexture.h"
#include "ShadowRenderTarget.h"

ShadowRenderTarget::ShadowRenderTarget()
	: mDepthStencilBuffer(nullptr)
	, mDepthStencilView(nullptr)
	, mDestBuffer(nullptr)
{
}

ShadowRenderTarget::~ShadowRenderTarget()
{
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_DELETE(mDestBuffer);
}

bool ShadowRenderTarget::Create(int w, int h)
{
	bool ret = false;
	RenderTexture* rt = new RenderTexture();
	CHECK(rt && rt->Create(w, h));
	mDestBuffer = rt;

	D3D11_TEXTURE2D_DESC depthDesc;
	mDestBuffer->mBuffer->GetDesc(&depthDesc);
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	CHECK(SUCCEEDED(gCore.GetDevice()->CreateTexture2D(&depthDesc, NULL, &mDepthStencilBuffer)));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;
	CHECK(SUCCEEDED(gCore.GetDevice()->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, &mDepthStencilView)));

	ret = true;
Exit0:
	return ret;
}

void ShadowRenderTarget::Begin(const Color& clearColor)
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		context->OMSetRenderTargets(1, &mDestBuffer->mView, mDepthStencilView);
		context->ClearRenderTargetView(mDestBuffer->mView, (float*)&clearColor);
		context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void ShadowRenderTarget::End()
{
}

void ShadowRenderTarget::SetSlot(int slot, bool close)
{
	mDestBuffer->SetSlot(slot, close);
}
