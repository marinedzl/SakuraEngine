#include "stdafx.h"
#include "Core.h"
#include "RenderTexture.h"
#include "RenderWindow.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget()
	: mDepthStencilBuffer(nullptr)
	, mDepthStencilView(nullptr)
	, mDestBuffer(nullptr)
	, mDepthBuffer(nullptr)
	, mGBuffer(nullptr)
	, mWidth(0)
	, mHeight(0)
{
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_DELETE(mDestBuffer);
	SAFE_DELETE(mDepthBuffer);
	SAFE_DELETE(mGBuffer);
}

void RenderTarget::Release()
{
	delete this;
}

void RenderTarget::Begin(const Color& clearColor)
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		context->OMSetRenderTargets(1, &mDestBuffer->mView, mDepthStencilView);
		context->ClearRenderTargetView(mDestBuffer->mView, (float*)&clearColor);
		context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		mGBuffer->SetSlot(3);
	}
}

void RenderTarget::Present()
{
	mGBuffer->SetSlot(3, true);
	mDestBuffer->Present();
}

void RenderTarget::BeginDepth()
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		Color clearColor(0, 0, 0, 0);
		context->OMSetRenderTargets(1, &mDepthBuffer->mView, mDepthStencilView);
		context->ClearRenderTargetView(mDepthBuffer->mView, (float*)&clearColor);
		context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void RenderTarget::BeginGBuffer()
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		Color clearColor(0, 0, 0, 1);
		context->OMSetRenderTargets(1, &mGBuffer->mView, mDepthStencilView);
		context->ClearRenderTargetView(mGBuffer->mView, (float*)&clearColor);
		context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		mDepthBuffer->SetSlot(0);
	}
}

bool RenderTarget::CaptureToFile(const char * filename)
{
	std::string name = filename;
	name += "_d.bmp";
	mDepthBuffer->CaptureToFile(name.c_str());
	name = filename;
	name += "_g.bmp";
	mGBuffer->CaptureToFile(name.c_str());
	return true;
}

bool RenderTarget::Create(HWND hWnd, int w, int h)
{
	bool ret = false;
	CHECK(hWnd);

	mWidth = (float)w;
	mHeight = (float)h;

	RenderWindow* rw = new RenderWindow();
	CHECK(rw && rw->Create(hWnd));
	mDestBuffer = rw;

	RenderTexture* rt = new RenderTexture();
	CHECK(rt && rt->Create(w, h));
	mDepthBuffer = rt;

	rt = new RenderTexture();
	CHECK(rt && rt->Create(w, h));
	mGBuffer = rt;

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
