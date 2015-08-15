#include "stdafx.h"
#include "TestMan.h"
#include "CameraController.h"
#include "CharaCtrl.h"

CharaCtrl::CharaCtrl(GameObject& gameObject, GameObject* weapon)
	: gameObject(gameObject)
	, transform(gameObject.GetTransform())
	, weapon(weapon)
	, mGravity(9.8f)
	, mMoveSpeed(4.0f)
	, mRotateSpeed(10)
	, mJumpGravity(20)
	, mJumpInitSpeed(10)
{
	animator = gameObject.GetAnimator();
	mCCT = gameObject.GetCCT();
	if (weapon)
	{
		weaponOffset = weapon->GetTransform();
		weaponOffset.scaling = Vector3(1, 1, 1);
	}
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
		Vector3 delta = mDest - transform.position;
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
	}

	Vector3 offset(0, 2, 0);
	gTestMan.GetCamera()->FocusOn(transform.position + offset);

	if (weapon)
	{
		Matrix bindPos;
		animator->GetBoneTM("wuqi_R", bindPos);
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
	if (mState != eMove && mState != eIdle)
		return;
	mDest = dest;
	mState = eMove;
	animator->SetBool("run", true);
	animator->SetBool("stop", false);
	animator->SetBool("jump", false);
}

void CharaCtrl::Stop()
{
	mState = eIdle;
	animator->SetBool("run", false);
	animator->SetBool("stop", true);
	animator->SetBool("jump", false);
}

void CharaCtrl::Jump()
{
	if (mState != eMove && mState != eIdle)
		return;
	mState = eJump;
	mJumpSpeed = mDest - transform.position;
	mJumpSpeed.y = mJumpInitSpeed;
	animator->SetBool("run", false);
	animator->SetBool("stop", false);
	animator->SetBool("jump", true);
	animator->SetBool("land", false);
}

void CharaCtrl::Land()
{
	mState = eIdle;
	animator->SetBool("jump", false);
	animator->SetBool("land", true);
}
