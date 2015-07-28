#pragma once

class Core
{
public:
	Core();
	~Core();
	bool Init(const char* resourePath);
	void Release();
	ID3D11Device* GetDevice() { return mDevice; }
	ID3D11DeviceContext* GetContext() { return mContext; }
	IDXGIFactory1* GetDxgiFactory() { return mDxgiFactory; }
	const char* GetWorkpath() const { return mWorkpath.c_str(); }
private:
	bool CreateDevice();
private:
	std::string mWorkpath;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	IDXGIFactory1* mDxgiFactory;
};

extern Core gCore;
