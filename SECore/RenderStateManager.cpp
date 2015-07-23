#include "stdafx.h"
#include "Core.h"
#include "RenderStateManager.h"

RenderStateManager gRenderStateManager;

RenderStateManager::RenderStateManager()
{
}

RenderStateManager::~RenderStateManager()
{
}

void RenderStateManager::Release()
{
	for (int i = 0; i < RS_Count; ++i)
		delete mRenderStateProxy[i];
}

bool RenderStateManager::Init()
{
	mRenderStateProxy[RS_BlendState] = new RenderStateProxy(new RenderState<ID3D11BlendState>());
	mRenderStateProxy[RS_RasterizerState] = new RenderStateProxy(new RenderState<ID3D11RasterizerState>());
	mRenderStateProxy[RS_DepthStencilState] = new RenderStateProxy(new RenderState<ID3D11DepthStencilState>());
	return true;
}

ID3D11RasterizerState* RenderStateManager::GetRasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AntialiasedLineEnable = FALSE;
	desc.CullMode = cull;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = FALSE;
	desc.FillMode = fill;
	desc.FrontCounterClockwise = FALSE; // watch this!
	desc.MultisampleEnable = FALSE;
	desc.ScissorEnable = FALSE;
	desc.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* ret = GetRenderState<ID3D11RasterizerState>(desc);

	return ret;
}
