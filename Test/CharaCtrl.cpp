#include "stdafx.h"
#include "TestMan.h"
#include "CameraController.h"
#include "CharaCtrl.h"

CharaCtrl::CharaCtrl(GameObject* gameObject)
	: gameObject(gameObject)
	, mGravity(9.8f)
	, mMoveSpeed(4.0f)
	, mRotateSpeed(10)
{
	animation = gameObject->GetAnimation();
	mCCT = gameObject->GetCCT();
}

CharaCtrl::~CharaCtrl()
{
}

void CharaCtrl::LookAt(const Vector3& delta, float lerp)
{
	float angle = XM_PI + XM_PIDIV2 - atan2f(delta.z, delta.x);
	XMVECTOR dst = XMQuaternionRotationRollPitchYaw(0, angle, 0);
	XMVECTOR src = XMLoadFloat4((XMFLOAT4*)&gameObject->GetTransform().rotation);
	XMVECTOR rot = XMQuaternionSlerp(src, dst, lerp);
	XMStoreFloat4((XMFLOAT4*)&gameObject->GetTransform().rotation, rot);
}

void CharaCtrl::Update(float deltaTime)
{
	switch (mState)
	{
	case eIdle:
	{
		Vector3 motion(0, -mGravity, 0);
		motion *= deltaTime;
		mCCT->Move(motion, deltaTime);
	}
		break;
	case eMove:
	{
		Vector3 delta = mDest - gameObject->GetTransform().position;
		delta.y = 0;
		XMVECTOR v = XMLoadFloat3((XMFLOAT3*)&delta);
		LookAt(delta, mRotateSpeed * deltaTime);
		XMVECTOR vLength = XMVector3Length(v);
		float distance = 0;
		XMStoreFloat(&distance, vLength);
		if (distance > 0.1f)
		{
			v = XMVector3Normalize(v);
			v *= mMoveSpeed * deltaTime;
			Vector3 motion;
			XMStoreFloat3((XMFLOAT3*)&motion, v);
			motion.y = -mGravity * deltaTime;
			mCCT->Move(motion, deltaTime);
		}
		else
		{
			Stop();
		}
	}
		break;
	}

	Vector3 offset(0, 2, 0);
	gTestMan.GetCamera()->FocusOn(gameObject->GetTransform().position + offset);
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
				if (hit.entity != gameObject)
				{
					Vector3 dest = hit.point;
					dest.y = gameObject->GetTransform().position.y;
					MoveTo(dest);
				}
			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
	}
	break;
	case WM_MOUSEMOVE:
	{

	}
	break;
	}
}

void CharaCtrl::MoveTo(const Vector3 & dest)
{
	mDest = dest;
	mState = eMove;
	animation->CrossFade("run", 0.2f);
}

void CharaCtrl::Stop()
{
	mState = eIdle;
	animation->CrossFade("idle", 0.2f);
}
