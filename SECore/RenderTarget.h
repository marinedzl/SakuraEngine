#pragma once

class RenderTarget : public SECore::RenderTarget
{
public:
	RenderTarget();
	virtual ~RenderTarget();
	virtual void Release();
	virtual bool Begin();
	virtual void End();
	virtual void ClearDepth();
	virtual float GetWidth() const { return mWidth; }
	virtual float GetHeight() const { return mHeight; }
	bool Create(ID3D11Texture2D* target);
private:
	bool Rebuild();
private:
	float mWidth;
	float mHeight;
	ID3D11Texture2D* mRenderTargetBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
};
