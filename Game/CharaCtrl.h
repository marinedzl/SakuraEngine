#pragma once

class CharaCtrl
{
public:
	CharaCtrl(Scene* scene, GameObject& gameObject, GameObject* weapon, Light* light);
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
	void OnClipChanged(const char* prev, const char* curr);
	bool IsCoolDown() const { return mCoolDown <= 0; }
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
	Scene* scene;
	int mAttackStep;
	State mState;
	Vector3 mDest;
	GameObject& gameObject;
	GameObject* weapon;
	Light* light;
	Transform& transform;
	Transform weaponOffset;
	Animation& animation;
	CharacterController* mCCT;
	float mMoveSpeed;
	float mDashSpeedFactor;
	float mRotateSpeed;
	float mGravity;
	float mJumpInitSpeed;
	Vector3 mJumpSpeed;
	float mJumpGravity;
	float mCoolDown;
};
