#pragma once
#include "RenderTargetView.h"

class RenderTexture : public RenderTargetView
{
public:
	RenderTexture();
	virtual ~RenderTexture();
	bool Create(int w, int h);
	void SetSlot(int slot, bool close = false);
	bool CaptureToFile(const char* filename);
private:
	D3D11_TEXTURE2D_DESC mDesc;
	ID3D11Texture2D* mPtr;
	ID3D11ShaderResourceView* mSRV;
	ID3D11SamplerState* mSampler;
};
