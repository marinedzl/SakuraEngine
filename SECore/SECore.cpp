#include "stdafx.h"
#include "Scene.h"
#include "Core.h"
#include "RenderWindow.h"
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
}
