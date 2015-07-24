#pragma once

class TestMan
{
public:
	TestMan();
	virtual ~TestMan();
	void Release();
	void Init();
	void CreateWnd(HWND hWnd);
	void Process();
private:
	SECore::Scene* mScene;
	SECore::RenderTarget* mRT;
};

extern TestMan gTestMan;
