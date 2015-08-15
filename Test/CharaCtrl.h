#pragma once

class CharaCtrl
{
public:
	CharaCtrl(GameObject& gameObject, GameObject* weapon);
	virtual ~CharaCtrl();
	void Update(float deltaTime);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MoveTo(const Vector3& dest);
	void LookAt(const Vector3& delta, float lerp);
	void Stop();
	void Jump();
	void Run();
	void Dash();
	void Land();
	void Attack();
	void OnAnimatorCallback();
private:
	void UpdateMove(float deltaTime, float factor);
private:
	enum State
	{
		eIdle,
		eRun,
		eDash,
		eJump,
		eAttack,
	};
private:
	State mState;
	Vector3 mDest;
	GameObject& gameObject;
	Transform& transform;
	GameObject* weapon;
	Transform weaponOffset;
	Animator* animator;
	CharacterController* mCCT;
	float mMoveSpeed;
	float mDashSpeedFactor;
	float mRotateSpeed;
	float mGravity;
	float mJumpInitSpeed;
	Vector3 mJumpSpeed;
	float mJumpGravity;
};
