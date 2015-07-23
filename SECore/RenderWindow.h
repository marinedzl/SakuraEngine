#pragma once
#include "RenderTarget.h"

class RenderWindow : public RenderTarget
{
public:
	RenderWindow();
	virtual ~RenderWindow();
	virtual void Release();
	virtual void End();
	bool Create(HWND hWnd);
private:
	IDXGISwapChain* mSwapChain;
};
