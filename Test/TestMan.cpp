#include "stdafx.h"
#include "CharaCtrl.h"
#include "CameraController.h"
#include "TestMan.h"

TestMan gTestMan;

TestMan::TestMan()
	: scene(nullptr)
	, mRT(nullptr)
	, mCamera(nullptr)
	, mCharaCtrl(nullptr)
{
}

TestMan::~TestMan()
{
}

void TestMan::Release()
{
	SAFE_DELETE(mCharaCtrl);
	SAFE_DELETE(mCamera);
	SAFE_RELEASE(mRT);
	SAFE_RELEASE(scene);
	SECore::ReleaseCore();
}

void TestMan::Init()
{
	char buff[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buff);

	std::string workpath = buff;
	workpath += "\\";

	CHECK(SECore::InitCore(workpath.c_str()));
	scene = SECore::CreateScene();
	CHECK(scene);

	mCamera = new CameraController(scene->GetCamera());

	CHECK(scene->LoadAdditive("scene.json"));

	GameObject* mainChara = scene->FindEntity("main chara");
	GameObject* weapon = scene->FindEntity("weapon");

	if (mainChara)
	{
		mCharaCtrl = new CharaCtrl(*mainChara, weapon);
	}

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
	DWORD dwDelta = currentTime - mLastTime;

	if (dwDelta <= 0)
		return;

	mLastTime = currentTime;

	float deltaTime = dwDelta / 1000.0f;

	//deltaTime *= 0.1f;

	if (mCamera)
		mCamera->Update(deltaTime);

	if (scene)
		scene->Update(deltaTime);

	if (mCharaCtrl)
		mCharaCtrl->Update(deltaTime);

	if (mRT)
	{
		mRT->Begin();
		if (scene)
		{
			scene->Draw(mRT);
		}
		mRT->End();
	}
}

void TestMan::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Vector2 pos((float)(short)LOWORD(lParam), (float)(short)HIWORD(lParam));

	if (mCharaCtrl)
	{
		mCharaCtrl->WndProc(hWnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_MBUTTONDOWN:
	{
		//mCamera->Begin(pos.x, pos.y, CameraController::eOpMove);
		//SetCapture(hWnd);
	}
	break;
	case WM_MBUTTONUP:
	{
		//mCamera->End();
		//ReleaseCapture();
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
