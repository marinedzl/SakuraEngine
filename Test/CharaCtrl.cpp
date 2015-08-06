#include "stdafx.h"
#include "CharaCtrl.h"

CharaCtrl::CharaCtrl(GameObject* gameObject)
	: gameObject(gameObject)
{
	animation = gameObject->GetAnimation();
}

CharaCtrl::~CharaCtrl()
{
}

void CharaCtrl::Update(float deltaTime)
{

}

void CharaCtrl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Vector2 pos((float)(short)LOWORD(lParam), (float)(short)HIWORD(lParam));

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		Run();
	}
	break;
	case WM_LBUTTONUP:
	{
		Stop();
	}
	break;
	case WM_MOUSEMOVE:
	{

	}
	break;
	}
}

void CharaCtrl::Run()
{
	animation->CrossFade("run", 0.2f);
}

void CharaCtrl::Stop()
{
	animation->CrossFade("idle", 0.2f);
}
