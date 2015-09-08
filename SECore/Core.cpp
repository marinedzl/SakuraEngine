#include "stdafx.h"
#include "RenderStateManager.h"
#include "MeshRenderer.h"
#include "GizmoRenderer.h"
#include "ResourceManager.h"
#include "Physics.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Scene.h"
#include "RenderWindow.h"
#include "Camera.h"
#include "Core.h"

Core gCore;

Core::Core()
	: mScene(nullptr)
{
}

Core::~Core()
{
}

bool Core::Init()
{
	bool ret = false;

	FreeImage_Initialise(TRUE);

	CHECK(CreateDevice());
	CHECK(gResourceManager.Init());
	CHECK(gRenderStateManager.Init());
	CHECK(gGizmosRenderer.Init());
	CHECK(gMeshRenderer.Init());
	CHECK(gPhysicsCore.Init());
	CHECK(gPointLightShader.Init());
	CHECK(gDirectionalLightShader.Init());

	ret = true;
Exit0:
	return true;
}

void Core::Release()
{
	gDirectionalLightShader.Release();
	gPointLightShader.Release();
	gPhysicsCore.Release();
	gMeshRenderer.Release();
	gGizmosRenderer.Release();
	gRenderStateManager.Release();
	gResourceManager.Release();

	FreeImage_DeInitialise();

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

SECore::Scene* Core::CreateScene(void)
{
	if (Scene* scene = new Scene())
	{
		if (!scene->Init())
		{
			delete scene;
			scene = nullptr;
		}
		else
		{
			return scene;
		}
		mScene = scene;
	}
	return nullptr;
}

SECore::RenderTarget* Core::CreateRenderTarget(HWND hWnd)
{
	if (RenderWindow* wnd = new RenderWindow())
	{
		if (!wnd->Create(hWnd))
		{
			delete wnd;
			wnd = nullptr;
		}
		else
		{
			return wnd;
		}
	}
	return nullptr;
}

SECore::Texture* Core::LoadTexture(const char* name)
{
	return gResourceManager.LoadTexture(name);
}

SECore::Mesh* Core::LoadMesh(const char* name)
{
	return gResourceManager.LoadMesh(name);
}

SECore::AnimationClip* Core::LoadClip(const char* name)
{
	return gResourceManager.LoadClip(name);
}

SECore::Skeleton* Core::LoadSkeleton(const char* name)
{
	return gResourceManager.LoadSkeleton(name);
}

SECore::Shader* Core::CreateShader(const char* name)
{
	return gResourceManager.CreateShader(name);
}

SECore::Camera * Core::CreateCamera()
{
	return new Camera();
}

SECore::Scene* Core::GetScene()
{
	return mScene;
}
