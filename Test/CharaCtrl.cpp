#include "stdafx.h"
#include "TestMan.h"
#include "CameraCtrl.h"
#include "CharaCtrl.h"

namespace
{
	void _ClipChangedCallback(void* data, const char* prev, const char* curr)
	{
		((CharaCtrl*)data)->OnClipChanged(prev, curr);
	}
}

CharaCtrl::CharaCtrl(GameObject& gameObject, GameObject* weapon)
	: gameObject(gameObject)
	, weapon(weapon)
	, transform(gameObject.GetTransform())
	, mGravity(9.8f)
	, mMoveSpeed(5.5f)
	, mDashSpeedFactor(1.5f)
	, mRotateSpeed(10)
	, mJumpGravity(20)
	, mJumpInitSpeed(10)
	, animation(*gameObject.GetAnimation())
	, mCoolDown(0)
{
	mCCT = gameObject.GetCCT();
	animation.Play("idle");
	animation.SetClipChangedCallback(_ClipChangedCallback, this);
}

CharaCtrl::~CharaCtrl()
{
}

void CharaCtrl::LookAt(const Vector3& delta, float lerp)
{
	float angle = XM_PI + XM_PIDIV2 - atan2f(delta.z, delta.x);
	XMVECTOR dst = XMQuaternionRotationRollPitchYaw(0, angle, 0);
	XMVECTOR src = XMLoadFloat4((XMFLOAT4*)&transform.rotation);
	XMVECTOR rot = XMQuaternionSlerp(src, dst, lerp);
	XMStoreFloat4((XMFLOAT4*)&transform.rotation, rot);
}

inline XMMATRIX AffineTransform(const Vector3& position, const Quat& rotation, const Vector3& scaling)
{
	XMMATRIX mat;
	XMVECTOR p = XMLoadFloat3((const XMFLOAT3*)&position);
	XMVECTOR q = XMLoadFloat4((const XMFLOAT4*)&rotation);
	XMVECTOR s = XMLoadFloat3((const XMFLOAT3*)&scaling);
	mat = XMMatrixAffineTransformation(s, XMQuaternionIdentity(), q, p);
	return mat;
}

inline XMMATRIX AffineTransform(const Transform& transform)
{
	return AffineTransform(transform.position, transform.rotation, transform.scaling);
}

inline bool MatrixDecompose(const XMMATRIX& mat, Vector3& position, Quat& rotation, Vector3& scaling)
{
	XMVECTOR p, q, s;
	if (XMMatrixDecompose(&s, &q, &p, mat))
	{
		XMStoreFloat3((XMFLOAT3*)&position, p);
		XMStoreFloat4((XMFLOAT4*)&rotation, q);
		XMStoreFloat3((XMFLOAT3*)&scaling, s);
		return true;
	}
	else
		return false;
}

inline bool MatrixDecompose(const XMMATRIX& mat, Transform& transform)
{
	return MatrixDecompose(mat, transform.position, transform.rotation, transform.scaling);
}

void CharaCtrl::UpdateMove(float deltaTime, float factor)
{
	Vector3 delta = mDest - transform.position;
	delta.y = 0;
	XMVECTOR v = XMLoadFloat3((XMFLOAT3*)&delta);
	LookAt(delta, mRotateSpeed * factor * deltaTime);
	XMVECTOR vLength = XMVector3Length(v);
	float distance = 0;
	XMStoreFloat(&distance, vLength);
	if (distance > 0.1f)
	{
		v = XMVector3Normalize(v);
		v *= mMoveSpeed * factor * deltaTime;
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

void CharaCtrl::Update(float deltaTime)
{
	mCoolDown -= deltaTime;
	mCoolDown = max(mCoolDown, 0);

	switch (mState)
	{
	case eIdle:
	{
		Vector3 motion(0, -mGravity, 0);
		motion *= deltaTime;
		mCCT->Move(motion, deltaTime);
	}
	break;
	case eRun:
	{
		UpdateMove(deltaTime, 1);

		if (KEY_DOWN(VK_LSHIFT))
		{
			Dash();
		}
	}
	break;
	case eDash:
	{
		UpdateMove(deltaTime, mDashSpeedFactor);

		if (!KEY_DOWN(VK_LSHIFT))
		{
			Run();
		}
	}
	break;
	case eJump:
	{
		mJumpSpeed.y -= mJumpGravity * deltaTime;
		Vector3 motion = mJumpSpeed * deltaTime;
		if (mCCT->Move(motion, deltaTime))
		{
			Land();
		}
	}
	break;
	case eAttack:
	{

	}
		break;
	}

	Vector3 offset(0, 2, 0);
	gTestMan.GetCameraCtrl()->FocusOn(transform.position + offset);

	if (weapon)
	{
		Matrix bindPos;
		animation.GetSavedBoneTM("wuqi_R", bindPos);
		XMMATRIX world = AffineTransform(transform);
		XMMATRIX bone = XMLoadFloat4x4((XMFLOAT4X4*)&bindPos);
		XMMATRIX offset = AffineTransform(weaponOffset);
		world = offset * bone * world;
		MatrixDecompose(world, weapon->GetTransform());
	}
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
				if (hit.entity != &gameObject)
				{
					Vector3 dest = hit.point;
					dest.y = transform.position.y;
					MoveTo(dest);
				}
			}
		}
	}
	break;
	case WM_RBUTTONDOWN:
	{
		Attack();
	}
	break;
	case WM_CHAR:
	case WM_SYSCHAR:
	{
		switch ((char)wParam)
		{
		case ' ':
		{
			Jump();
		}
		break;
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

	switch (mState)
	{
	case eIdle:
	{
		if (KEY_DOWN(VK_LSHIFT))
			Dash();
		else
			Run();
	}
	break;
	case eRun:
	{
		if (KEY_DOWN(VK_LSHIFT))
			Dash();
	}
	break;
	case eDash:
	{
		if (!KEY_DOWN(VK_LSHIFT))
			Run();
	}
	break;
	default:
		break;
	}
}

void CharaCtrl::Run()
{
	mState = eRun;
	animation.CrossFade("run", 0.2f);
}

void CharaCtrl::Dash()
{
	mState = eDash;
	animation.CrossFade("dash", 0.2f);
}

void CharaCtrl::Stop()
{
	mState = eIdle;
	animation.CrossFade("idle", 0.2f);
}

void CharaCtrl::Jump()
{
	if (mState != eRun && mState != eIdle)
		return;
	mState = eJump;
	mJumpSpeed = mDest - transform.position;
	mJumpSpeed.y = mJumpInitSpeed;
	animation.CrossFade("jump_takeoff", 0.3f, false);
	animation.CrossFadeQueue("jump_mid", 1, 0);
}

void CharaCtrl::Land()
{
	mState = eIdle;
	animation.CrossFade("jump_land", 0.1f, false);
	animation.CrossFadeQueue("idle", 1, 0);
}

void CharaCtrl::Attack()
{
	if (mState == eJump)
		return;

	if (!IsCoolDown())
		return;

	mCoolDown = 0.5f;

	mState = eAttack;
	animation.CrossFade("lattack1", 0.2f, false);
	animation.CrossFadeQueue("idle", 0.7f, 0.3f);
}

void CharaCtrl::OnClipChanged(const char * prev, const char * curr)
{
	if (strcmp(curr, "idle") == 0)
	{
		mState = eIdle;
	}
}
