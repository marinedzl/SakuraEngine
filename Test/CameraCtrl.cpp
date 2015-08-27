#include "stdafx.h"
#include "CameraCtrl.h"

CameraCtrl::CameraCtrl(Camera* camera)
	: mCamera(camera)
	, mOp(eOpNone)
	, mLookAt(0, 0, 0)
	, mUp(0, 1, 0)
	, mDistance(10)
	, mMinDistance(0.1f)
	, mMaxDistance(100.0f)
	, mYawPitch(XM_PIDIV4, -0.5f)
{

}

CameraCtrl::~CameraCtrl()
{
}

void CameraCtrl::Update(float deltaTime)
{
	XMMATRIX rotation = XMMatrixRotationY(mYawPitch.x) * XMMatrixRotationX(mYawPitch.y);
	XMMATRIX invRotation = XMMatrixInverse(nullptr, rotation);
	XMVECTOR vWorldUp, vWorldAhead;
	XMVECTOR vLocalUp = XMLoadFloat3(&XMFLOAT3(0, 1, 0));
	XMVECTOR vLocalAhead = XMLoadFloat3(&XMFLOAT3(0, 0, 1));
	vWorldUp = XMVector3TransformCoord(vLocalUp, invRotation);
	vWorldAhead = XMVector3TransformCoord(vLocalAhead, invRotation);
	vWorldAhead *= mDistance;
	
	Vector3 worldAhead;
	XMStoreFloat3((XMFLOAT3*)&worldAhead, vWorldAhead);
	XMStoreFloat3((XMFLOAT3*)&mUp, vWorldUp);

	Vector3 lookAt = mLookAt;
	Vector3 eye = lookAt - worldAhead;

	mCamera->SetEye(eye);
	mCamera->SetLookAt(mLookAt);
	mCamera->SetUp(mUp);
}

void CameraCtrl::Begin(float x, float y, Operator op)
{
	mOp = op;
	mDownPoint.x = x;
	mDownPoint.y = y;

	switch (mOp)
	{
	case eOpRotate:
	{
		mDownYawPitch = mYawPitch;
	}
	break;
	case eOpMove:
	{

	}
	break;
	}
}

void CameraCtrl::Move(float x, float y)
{
	mCurrentPoint.x = x;
	mCurrentPoint.y = y;

	switch (mOp)
	{
	case eOpRotate:
	{
		mYawPitch = mDownYawPitch + (mDownPoint - mCurrentPoint) * XM_1DIV2PI * 0.05f;
	}
		break;
	case eOpMove:
	{
		Vector3 screen;
		mCamera->WorldToScreen(mLookAt, screen);
		screen.x -= (mCurrentPoint.x - mDownPoint.x);
		screen.y -= (mCurrentPoint.y - mDownPoint.y);
		mCamera->ScreenToWorld(screen, mLookAt);
		mDownPoint = mCurrentPoint;
	}
		break;
	}
}

void CameraCtrl::End()
{
	mOp = eOpNone;
}

void CameraCtrl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Vector2 pos((float)(short)LOWORD(lParam), (float)(short)HIWORD(lParam));

	switch (message)
	{
	case WM_MBUTTONDOWN:
	{
		//Begin(pos.x, pos.y, eOpMove);
		//SetCapture(hWnd);s
		Begin(pos.x, pos.y, eOpRotate);
		SetCapture(hWnd);
	}
	break;
	case WM_RBUTTONDOWN:
	{
	}
	break;
	case WM_MOUSEMOVE:
	{
		Move(pos.x, pos.y);
	}
	break;
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	{
		End();
		ReleaseCapture();
	}
	break;
	case WM_MOUSEWHEEL:
	{
		short zDelta = (short)HIWORD(wParam);
		Scroll(-(float)zDelta * 0.01f);
	}
	break;
	}
}

void CameraCtrl::Scroll(float delta)
{
	mDistance += delta;
	mDistance = __min(mMaxDistance, mDistance);
	mDistance = __max(mMinDistance, mDistance);
}

void CameraCtrl::FocusOn(Vector3 v)
{
	mLookAt = v;
}
