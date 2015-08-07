#include "stdafx.h"
#include "RenderStateManager.h"
#include "MeshRenderer.h"
#include "GizmoRenderer.h"
#include "ResourceManager.h"
#include "Physics.h"
#include "Core.h"

Core gCore;

Core::Core()
{
}

Core::~Core()
{
}

bool Core::Init(const char* resourePath)
{
	bool ret = false;

	mWorkpath = resourePath;

	CHECK(CreateDevice());
	CHECK(gResourceManager.Init());
	CHECK(gRenderStateManager.Init());
	CHECK(gGizmosRenderer.Init());
	CHECK(gMeshRenderer.Init());
	CHECK(gPhysicsCore.Init());

	ret = true;
Exit0:
	return true;
}

void Core::Release()
{
	gPhysicsCore.Release();
	gMeshRenderer.Release();
	gGizmosRenderer.Release();
	gRenderStateManager.Release();
	gResourceManager.Release();

	SAFE_RELEASE(mDxgiFactory);
	SAFE_RELEASE(mContext);
	SAFE_RELEASE(mDevice);
}

bool Core::CreateDevice()
{
	bool ret = false;
	HRESULT hr = S_OK;
	std::vector<IDXGIAdapter1*> adapters;
	IDXGIAdapter1* adapter = nullptr;
	DWORD deviceFlags = (D3D11_CREATE_DEVICE_FLAG)(D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT);
	D3D_FEATURE_LEVEL deviceFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT i = 0;

	const D3D_FEATURE_LEVEL supportedFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&mDxgiFactory));
	CHECK(SUCCEEDED(hr));

	while (mDxgiFactory->EnumAdapters1(i++, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(adapter);
	}

	for (i = 0; i < adapters.size(); ++i)
	{
		hr = D3D11CreateDevice(
			adapters[i],
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			deviceFlags,
			supportedFeatureLevels,
			ARRAYSIZE(supportedFeatureLevels),
			D3D11_SDK_VERSION,
			&mDevice,
			&deviceFeatureLevel,
			&mContext);

		if (SUCCEEDED(hr))
			break;

		SAFE_RELEASE(mDevice);
		SAFE_RELEASE(mContext);
	}

	for (i = 0; i < adapters.size(); ++i)
		adapters[i]->Release();

	CHECK(mDevice && mContext);

	ret = true;
Exit0:
	return true;
}
