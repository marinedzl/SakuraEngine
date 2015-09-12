#pragma once

class CameraCtrl
{
public:
	enum Operator
	{
		eOpNone,
		eOpRotate,
		eOpMove,
	};
public:
	CameraCtrl(Camera* camera);
	virtual ~CameraCtrl();
	void Update(float deltaTime);
	void Begin(float x, float y, Operator op);
	void Move(float x, float y);
	void End();
	void Scroll(float delta);
	void FocusOn(Vector3 v);
	void SetPitch(float angle) { mYawPitch.y = angle; }
	void SetYaw(float angle) { mYawPitch.x = angle; }
private:
	Camera* mCamera;

	Operator mOp;

	Vector2 mDownYawPitch;
	Vector2 mYawPitch;
	Vector2 mDownPoint;
	Vector2 mCurrentPoint;

	float mMinDistance;
	float mMaxDistance;
	float mDistance;

	Vector3 mLookAt;
	Vector3 mUp;
};
