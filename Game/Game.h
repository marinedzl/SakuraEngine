#pragma once
#include "IGame.h"

class CharaCtrl;
class CameraCtrl;
class Game : public IGame
{
public:
	virtual ~Game();
	virtual void Release();
	virtual bool Init(SECore::Core* core);
	virtual bool EditorPlay(SECore::Scene* scene);
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Update(float deltaTime);
public:
	Game();
private:
	Scene* mScene;
	CameraCtrl* mCameraCtrl;
	GameObject* mMainChara;
	CharaCtrl* mCharaCtrl;
};
