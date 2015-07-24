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
	CHECK(SECore::InitCore());
Exit0:
	;
}

void TestMan::CreateWnd(HWND hWnd)
{
	mRT = SECore::CreateRenderTarget(hWnd);
}

void TestMan::Process()
{
	if (mRT)
	{
		mRT->Begin();
		mRT->End();
	}
}
