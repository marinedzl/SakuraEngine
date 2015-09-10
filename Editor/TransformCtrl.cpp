#include "stdafx.h"
#include "Editor.h"
#include "TransformCtrl.h"

TransformCtrl::TransformCtrl()
	: mOp(eOpNone)
{
}

TransformCtrl::~TransformCtrl()
{
}

void TransformCtrl::Attach(GameObject * gameObject)
{
	this->gameObject = gameObject;
}

void TransformCtrl::Update(float deltaTime)
{
}

void TransformCtrl::Begin(float x, float y, Operator op)
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

void TransformCtrl::Move(float x, float y)
{
	if (!gameObject)
		return;

	mCurrentPoint.x = x;
	mCurrentPoint.y = y;

	switch (mOp)
	{
	case eOpRotate:
	{
		mYawPitch = mDownYawPitch + (mDownPoint - mCurrentPoint) * XM_1DIV2PI * 0.05f;

		XMVECTOR q = XMQuaternionRotationRollPitchYaw(mYawPitch.x, mYawPitch.y, 0);
		XMStoreFloat4((XMFLOAT4*)&gameObject->GetTransform().rotation, q);
	}
	break;
	case eOpMove:
	{
		Vector3 screen;
		Vector3 pos = gameObject->GetTransform().position;
		theApp.camera->WorldToScreen(pos, screen);
		screen.x += (mCurrentPoint.x - mDownPoint.x);
		screen.y += (mCurrentPoint.y - mDownPoint.y);
		theApp.camera->ScreenToWorld(screen, pos);
		gameObject->SetPos(pos);
		mDownPoint = mCurrentPoint;
	}
	break;
	}
}

void TransformCtrl::End()
{
	mOp = eOpNone;
}
