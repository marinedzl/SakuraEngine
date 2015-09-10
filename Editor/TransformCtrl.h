#pragma once

class TransformCtrl
{
public:
	enum Operator
	{
		eOpNone,
		eOpRotate,
		eOpMove,
	};
public:
	TransformCtrl();
	virtual ~TransformCtrl();
	void Attach(GameObject* gameObject);
	void Update(float deltaTime);
	void Begin(float x, float y, Operator op);
	void Move(float x, float y);
	void End();
private:
	GameObject* gameObject;
	Operator mOp;
	Vector2 mDownYawPitch;
	Vector2 mYawPitch;
	float mRoll;
	Vector2 mDownPoint;
	Vector2 mCurrentPoint;
};
