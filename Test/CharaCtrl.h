#pragma once

class CharaCtrl
{
public:
	CharaCtrl(GameObject* gameObject);
	virtual ~CharaCtrl();
	void Update(float deltaTime);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void Run();
	void Stop();
private:
	GameObject* gameObject;
	Animation* animation;
	RigidBody* mRigidBody;
};
