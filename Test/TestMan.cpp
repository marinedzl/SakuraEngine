#include "stdafx.h"
#include "CharaCtrl.h"
#include "CameraCtrl.h"
#include "TestMan.h"

TestMan gTestMan;

void log(const char* fmt, ...)
{
	va_list args;
	size_t len;
	char buffer[1024];
	va_start(args, fmt);
	len = vsnprintf_s(buffer, 1024, fmt, args);
	va_end(args);
	printf(buffer);
	OutputDebugStringA(buffer);
}

TestMan::TestMan()
	: scene(nullptr)
	, mRT(nullptr)
	, mCameraCtrl(nullptr)
	, mCharaCtrl(nullptr)
{
}

TestMan::~TestMan()
{
}

void TestMan::Release()
{
	SAFE_DELETE(mCharaCtrl);
	SAFE_DELETE(mCameraCtrl);
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

	mCameraCtrl = new CameraCtrl(scene->GetCamera());

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

	if (mCameraCtrl)
		mCameraCtrl->Update(deltaTime);

	if (scene)
		scene->Update(deltaTime);

	if (mCharaCtrl)
		mCharaCtrl->Update(deltaTime);

	if (mRT)
	{
		if (scene)
		{
			scene->Draw(mRT);
		}
	}
}

void TestMan::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (mCharaCtrl)
		mCharaCtrl->WndProc(hWnd, message, wParam, lParam);

	if (mCameraCtrl)
		mCameraCtrl->WndProc(hWnd, message, wParam, lParam);
}
