#include "stdafx.h"
#include "Scene.h"
#include "Core.h"
#include "RenderWindow.h"
#include "SECore.h"

namespace SECore
{
	SECORE_API bool InitCore()
	{
		return gCore.Init();
	}

	SECORE_API void ReleaseCore()
	{
		gCore.Release();
	}

	SECORE_API Scene* CreateScene(void)
	{
		return new ::Scene();
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
