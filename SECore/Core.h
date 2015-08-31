#pragma once

class Core : public SECore::Core
{
public:
	virtual void Release();
	virtual SECore::Scene* CreateScene(void);
	virtual SECore::RenderTarget* CreateRenderTarget(HWND hWnd);
	virtual SECore::Texture* LoadTexture(const char* name);
	virtual SECore::Mesh* LoadMesh(const char* name);
	virtual SECore::AnimationClip* LoadClip(const char* name);
	virtual SECore::Skeleton* LoadSkeleton(const char* name);
	virtual SECore::Shader* CreateShader(const char* name);
public:
	Core();
	~Core();
	bool Init();
	ID3D11Device* GetDevice() { return mDevice; }
	ID3D11DeviceContext* GetContext() { return mContext; }
	IDXGIFactory1* GetDxgiFactory() { return mDxgiFactory; }
private:
	bool CreateDevice();
private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	IDXGIFactory1* mDxgiFactory;
};

extern Core gCore;
