#include "stdafx.h"
#include "Core.h"
#include "RenderTargetView.h"

RenderTargetView::RenderTargetView()
	: mBuffer(nullptr)
	, mView(nullptr)
{
}

RenderTargetView::~RenderTargetView()
{
	SAFE_RELEASE(mBuffer);
	SAFE_RELEASE(mView);
}

bool RenderTargetView::Create(ID3D11Texture2D* texture)
{
	bool ret = false;
	CHECK(texture);
	mBuffer = texture;
	mBuffer->AddRef();
	CHECK(Rebuild());
	ret = true;
Exit0:
	return ret;
}

bool RenderTargetView::Rebuild()
{
	bool ret = false;
	CHECK(mBuffer);

	D3D11_TEXTURE2D_DESC targetDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	mBuffer->GetDesc(&targetDesc);
	rtvDesc.Format = targetDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	rtvDesc.Texture2D.MipSlice = 0;
	CHECK(SUCCEEDED(gCore.GetDevice()->CreateRenderTargetView(mBuffer, &rtvDesc, &mView)));

	ret = true;
Exit0:
	return ret;
}
