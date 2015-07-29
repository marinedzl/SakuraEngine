#include "stdafx.h"
#include "CameraController.h"
#include "TestMan.h"

TestMan gTestMan;

TestMan::TestMan()
	: mScene(nullptr)
	, mRT(nullptr)
	, mCamera(nullptr)
{
}

TestMan::~TestMan()
{
}

void TestMan::Release()
{
	SAFE_DELETE(mCamera);
	SAFE_RELEASE(mRT);
	SAFE_RELEASE(mScene);
	SECore::ReleaseCore();
}

void TestMan::Init()
{
	char buff[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buff);

	std::string workpath = buff;
	workpath += "\\";

	CHECK(SECore::InitCore(workpath.c_str()));
	mScene = SECore::CreateScene();
	CHECK(mScene);

	mCamera = new CameraController(mScene->GetCamera());

	CHECK(mScene->LoadAdditive("scene.json"));

	mLastTime = timeGetTime();
Exit0:
	;
}

void TestMan::CreateWnd(HWND hWnd)
{
	mRT = SECore::CreateRenderTarget(hWnd);
}

void TestMan::Process()
{
	DWORD currentTime = timeGetTime();
	DWORD deltaTime = currentTime - mLastTime;
	mLastTime = currentTime;

	float d = deltaTime / 1000.0f;
	mCamera->Update(d);

	if (mScene)
	{
		mScene->Update(d);
	}

	if (mRT)
	{
		mRT->Begin();
		if (mScene)
		{
			mScene->Draw(mRT);
		}
		mRT->End();
	}
}

void TestMan::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Vector2 pos((float)(short)LOWORD(lParam), (float)(short)HIWORD(lParam));

	switch (message)
	{
	case WM_MBUTTONDOWN:
	{
		mCamera->Begin(pos.x, pos.y, CameraController::eOpMove);
		SetCapture(hWnd);
	}
	break;
	case WM_MBUTTONUP:
	{
		mCamera->End();
		ReleaseCapture();
	}
	break;
	case WM_RBUTTONDOWN:
	{
		mCamera->Begin(pos.x, pos.y, CameraController::eOpRotate);
		SetCapture(hWnd);
	}
	break;
	case WM_RBUTTONUP:
	{
		mCamera->End();
		ReleaseCapture();
	}
	break;
	case WM_MOUSEWHEEL:
	{
		short zDelta = (short)HIWORD(wParam);
		mCamera->Scroll(-(float)zDelta * 0.001f);
	}
	break;
	case WM_LBUTTONDOWN:
	{
	}
	break;
	case WM_LBUTTONUP:
	{
	}
	break;
	case WM_MOUSEMOVE:
	{
		mCamera->Move(pos.x, pos.y);
	}
	break;
	}
}
