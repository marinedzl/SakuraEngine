#pragma once

struct IGame
{
	virtual void Release() = 0;
	virtual bool Init(SECore::Core* core) = 0;
	virtual bool EditorPlay(SECore::Scene* scene) = 0;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Update(float deltaTime) = 0;
};
