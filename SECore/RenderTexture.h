#pragma once
#include "RenderTarget.h"

class RenderTexture : public RenderTarget
{
public:
	RenderTexture();
	virtual ~RenderTexture();
	virtual void Release();
	bool Create(int w, int h);
	void SetSlot(int slot, bool close = false);
	bool CaptureToFile(const char* filename);
private:
	int mWidth;
	int mHeight;
	ID3D11Texture2D* mPtr;
	ID3D11ShaderResourceView* mSRV;
	ID3D11SamplerState* mSampler;
};
