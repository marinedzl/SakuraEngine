#pragma once

class RenderTexture;
class ShadowRenderTarget
{
public:
	ShadowRenderTarget();
	virtual ~ShadowRenderTarget();
	bool Create(int w, int h);
	void Begin(const Color& clearColor);
	void End();
	void SetSlot(int slot, bool close = false);
private:
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	RenderTexture* mDestBuffer;
};
