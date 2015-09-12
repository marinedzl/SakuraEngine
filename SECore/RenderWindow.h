#pragma once
#include "RenderTargetView.h"

class RenderWindow : public RenderTargetView
{
public:
	RenderWindow();
	virtual ~RenderWindow();
	bool Create(HWND hWnd);
	void Present();
private:
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mBackBuffer;
};
