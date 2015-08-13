#pragma once

class CharaCtrl
{
public:
	CharaCtrl(GameObject* gameObject, GameObject* weapon);
	virtual ~CharaCtrl();
	void Update(float deltaTime);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MoveTo(const Vector3& dest);
	void LookAt(const Vector3& delta, float lerp);
	void Stop();
private:
	enum State
	{
		eIdle,
		eMove,
	};
private:
	State mState;
	Vector3 mDest;
	GameObject* gameObject;
	GameObject* weapon;
	Transform weaponOffset;
	Animation* animation;
	CharacterController* mCCT;
	float mMoveSpeed;
	float mRotateSpeed;
	float mGravity;
};
