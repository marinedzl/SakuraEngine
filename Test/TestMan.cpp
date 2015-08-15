#include "stdafx.h"
#include "CharaCtrl.h"
#include "CameraCtrl.h"
#include "TestMan.h"

TestMan gTestMan;

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
	if (mCharaCtrl)
		mCharaCtrl->WndProc(hWnd, message, wParam, lParam);

	if (mCameraCtrl)
		mCameraCtrl->WndProc(hWnd, message, wParam, lParam);
}
