#pragma once

class RenderTargetView
{
public:
	RenderTargetView();
	virtual ~RenderTargetView();
	bool Create(ID3D11Texture2D* target);
	bool Rebuild();
public:
	ID3D11Texture2D* mBuffer;
	ID3D11RenderTargetView* mView;
};
