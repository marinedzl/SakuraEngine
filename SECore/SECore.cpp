#include "stdafx.h"
#include "Scene.h"
#include "Core.h"
#include "RenderWindow.h"
#include "ResourceManager.h"
#include "SECore.h"

namespace SECore
{
	SECORE_API bool InitCore(const char* resourePath)
	{
		return gCore.Init(resourePath);
	}

	SECORE_API void ReleaseCore()
	{
		gCore.Release();
	}

	SECORE_API Scene* CreateScene(void)
	{
		if (::Scene* scene = new ::Scene())
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
		}
		return nullptr;
	}

	SECORE_API RenderTarget * CreateRenderTarget(HWND hWnd)
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

	SECORE_API Texture * LoadTexture(const char * name)
	{
		return gResourceManager.LoadTexture(name);
	}

	SECORE_API Mesh * LoadMesh(const char * name)
	{
		return gResourceManager.LoadMesh(name);
	}

	SECORE_API AnimationClip * LoadClip(const char * name)
	{
		return gResourceManager.LoadClip(name);
	}

	SECORE_API Skeleton * LoadSkeleton(const char * name)
	{
		return gResourceManager.LoadSkeleton(name);
	}

	SECORE_API Shader * CreateShader(const char * name)
	{
		return gResourceManager.CreateShader(name);
	}
}
