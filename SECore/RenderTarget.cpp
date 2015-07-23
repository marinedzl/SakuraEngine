#include "stdafx.h"
#include "Core.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget()
	: mRenderTargetBuffer(nullptr)
	, mRenderTargetView(nullptr)
	, mDepthStencilBuffer(nullptr)
	, mDepthStencilView(nullptr)
	, mWidth(0)
	, mHeight(0)
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Release()
{
	SAFE_RELEASE(mRenderTargetBuffer);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mDepthStencilBuffer);
	delete this;
}

void RenderTarget::ClearDepth()
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

bool RenderTarget::Begin()
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		//float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		float ClearColor[4] = { 1, 1, 1, 1 };
		context->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
		context->ClearRenderTargetView(mRenderTargetView, ClearColor);
		context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	return true;
}

void RenderTarget::End()
{

}

bool RenderTarget::Create(ID3D11Texture2D* target)
{
	bool ret = false;

	CHECK(target);

	mRenderTargetBuffer = target;

	Rebuild();

	ret = true;
Exit0:
	return ret;
}

bool RenderTarget::Rebuild()
{
	bool ret = false;

	CHECK(mRenderTargetBuffer);

	D3D11_TEXTURE2D_DESC targetDesc;
	mRenderTargetBuffer->GetDesc(&targetDesc);

	mWidth = (float)targetDesc.Width;
	mHeight = (float)targetDesc.Height;

	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.Width = targetDesc.Width;
	depthDesc.Height = targetDesc.Height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.SampleDesc.Count = targetDesc.SampleDesc.Count;
	depthDesc.SampleDesc.Quality = targetDesc.SampleDesc.Quality;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	CHECK(SUCCEEDED(gCore.GetDevice()->CreateTexture2D(&depthDesc, NULL, &mDepthStencilBuffer)));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	rtvDesc.Texture2D.MipSlice = 0;
	CHECK(SUCCEEDED(gCore.GetDevice()->CreateRenderTargetView(mRenderTargetBuffer, &rtvDesc, &mRenderTargetView)));

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
