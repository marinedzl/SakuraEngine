#pragma once

class CameraController
{
public:
	enum Operator
	{
		eOpNone,
		eOpRotate,
		eOpMove,
	};
public:
	CameraController(Camera* camera);
	virtual ~CameraController();
	void Update(float deltaTime);
	void Begin(float x, float y, Operator op);
	void Move(float x, float y);
	void End();
	void Scroll(float delta);
	void FocusOn(Vector3 v);
private:
	Camera* mCamera;

	Operator mOp;

	Vector2 mDownYawPitch;
	Vector2 mYawPitch;
	Vector2 mDownPoint;
	Vector2 mCurrentPoint;

	float mMinDistance;
	float mMaxDistance;
	float mBaseDistance;
	float mDistance;

	Vector3 mLookAt;
	Vector3 mEye;
	Vector3 mUp;
};
