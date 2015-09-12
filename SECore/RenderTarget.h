#pragma once

class RenderTexture;
class RenderWindow;
class RenderTarget : public SECore::RenderTarget
{
public:
	RenderTarget();
	virtual ~RenderTarget();
	virtual void Release();
	virtual float GetWidth() const { return mWidth; }
	virtual float GetHeight() const { return mHeight; }
	virtual bool CaptureToFile(const char* filename);
	bool Create(HWND hWnd, int w, int h);
	void BeginDepth();
	void BeginGBuffer();
	void Begin(const Color& clearColor);
	void Present();
private:
	float mWidth;
	float mHeight;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	RenderWindow* mDestBuffer;
	RenderTexture* mDepthBuffer;
	RenderTexture* mGBuffer;
};
