#include "stdafx.h"
#include "TestMan.h"
#include "CharaCtrl.h"

CharaCtrl::CharaCtrl(GameObject* gameObject)
	: gameObject(gameObject)
{
	animation = gameObject->GetAnimation();
	mRigidBody = gameObject->GetCollider()->GetRigidBody();
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
					mRigidBody->SetLinearVelocity(Vector3(0.01f, 0, 0));
				}
			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		mRigidBody->SetLinearVelocity(Vector3(0, 0, 0));
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
