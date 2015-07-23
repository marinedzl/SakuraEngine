#include "stdafx.h"
#include "Core.h"
#include "RenderWindow.h"

RenderWindow::RenderWindow()
	: mSwapChain(nullptr)
{
}

RenderWindow::~RenderWindow()
{
}

void RenderWindow::Release()
{
	SAFE_RELEASE(mSwapChain);
	RenderTarget::Release();
}

bool RenderWindow::Create(HWND hWnd)
{
	bool ret = false;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* target = nullptr;

	RECT rect;
	GetClientRect(hWnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;

	CHECK(hWnd);

	memset(&swapChainDesc, 0, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.OutputWindow = hWnd;

	CHECK(SUCCEEDED(gCore.GetDxgiFactory()->CreateSwapChain(gCore.GetDevice(), &swapChainDesc, &mSwapChain)));
	CHECK(SUCCEEDED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&target)));

	RenderTarget::Create(target);

	ret = true;
Exit0:
	return ret;
}

void RenderWindow::End()
{
	RenderTarget::End();

	if (mSwapChain)
		mSwapChain->Present(0, 0);
}
