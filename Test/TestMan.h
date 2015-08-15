#pragma once

class CameraCtrl;
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
	CameraCtrl* GetCameraCtrl() { return mCameraCtrl; }
public:
	SECore::Scene* scene;
private:
	SECore::RenderTarget* mRT;
	DWORD mLastTime;
	CameraCtrl* mCameraCtrl;
	CharaCtrl* mCharaCtrl;
};

extern TestMan gTestMan;
