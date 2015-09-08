#pragma once
#include "IGame.h"

class CharaCtrl;
class CameraCtrl;
class Game : public IGame
{
public:
	virtual ~Game();
	virtual void Release();
	virtual bool Init(SECore::Core* core, HWND hWnd);
	virtual bool EditorPlay(SECore::Scene* scene);
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Draw();
	virtual void Update(float deltaTime);
	Camera* GetCamera() { return mCamera; }
public:
	Game();
private:
	Core* mCore;
	Scene* mScene;
	Camera* mCamera;
	CameraCtrl* mCameraCtrl;
	GameObject* mMainChara;
	CharaCtrl* mCharaCtrl;
	RenderTarget* mRT;
};
extern Game gGame;
