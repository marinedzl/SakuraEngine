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
	void Land();
private:
	enum State
	{
		eIdle,
		eMove,
		eJump,
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
	float mRotateSpeed;
	float mGravity;
	float mJumpInitSpeed;
	Vector3 mJumpSpeed;
	float mJumpGravity;
};
