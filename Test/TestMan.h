#pragma once

class CameraController;
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
private:
	SECore::Scene* mScene;
	SECore::RenderTarget* mRT;
	DWORD mLastTime;
	CameraController* mCamera;
};

extern TestMan gTestMan;
