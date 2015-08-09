#include "stdafx.h"
#include "CameraController.h"

CameraController::CameraController(Camera* camera)
	: mCamera(camera)
	, mOp(eOpNone)
	, mLookAt(0, 0, 0)
	, mUp(0, 1, 0)
	, mDistance(10)
	, mMinDistance(0.1f)
	, mMaxDistance(100.0f)
	, mYawPitch(0, -0.5f)
{

}

CameraController::~CameraController()
{
}

void CameraController::Update(float deltaTime)
{
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(mYawPitch.y, mYawPitch.x, 0);
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

void CameraController::Begin(float x, float y, Operator op)
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

void CameraController::Move(float x, float y)
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

void CameraController::End()
{
	mOp = eOpNone;
}

void CameraController::Scroll(float delta)
{
	mDistance += delta;
	mDistance = __min(mMaxDistance, mDistance);
	mDistance = __max(mMinDistance, mDistance);
}

void CameraController::FocusOn(Vector3 v)
{
	mLookAt = v;
}
