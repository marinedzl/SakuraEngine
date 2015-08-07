#pragma once

class CameraController;
class CharaCtrl;
class TestMan
{
public:
	TestMan();
	virtual ~TestMan();
	void Release();
	void Init();
	void CreateWnd(HWND hWnd);
	void Process();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	SECore::Scene* scene;
private:
	SECore::RenderTarget* mRT;
	DWORD mLastTime;
	CameraController* mCamera;
	CharaCtrl* mCharaCtrl;
};

extern TestMan gTestMan;
