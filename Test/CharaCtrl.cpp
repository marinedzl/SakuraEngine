#include "stdafx.h"
#include "TestMan.h"
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
		if (gTestMan.scene)
		{
			SECore::Ray ray;
			SECore::RaycastHit hit;
			gTestMan.scene->GetCamera()->ScreenPointToRay(ray, Vector3(pos.x, pos.y, 0));
			if (gTestMan.scene->Raycast(ray, hit, 10000))
			{
				if (strcmp(hit.entity->GetName(), "main chara") == 0)
				{
					Run();
				}
			}
		}
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
