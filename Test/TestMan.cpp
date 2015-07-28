#include "stdafx.h"
#include "TestMan.h"

TestMan gTestMan;

TestMan::TestMan()
	: mScene(nullptr)
	, mRT(nullptr)
{
}

TestMan::~TestMan()
{
}

void TestMan::Release()
{
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
	CHECK(mScene->LoadAdditive("scene.json"));
Exit0:
	;
}

void TestMan::CreateWnd(HWND hWnd)
{
	mRT = SECore::CreateRenderTarget(hWnd);
}

void TestMan::Process()
{
	if (mScene)
	{
		mScene->Update(0);
	}

	if (mRT)
	{
		mRT->Begin();
		if (mScene)
		{
			mScene->Draw();
		}
		mRT->End();
	}
}
