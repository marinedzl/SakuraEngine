#pragma once

class CharaCtrl
{
public:
	CharaCtrl(GameObject* gameObject);
	virtual ~CharaCtrl();
	void Update(float deltaTime);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void MoveTo(const Vector3& dest);
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
	Animation* animation;
	CharacterController* mCCT;
	float mMoveSpeed;
	float mGravity;
};
