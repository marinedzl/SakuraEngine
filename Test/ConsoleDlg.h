#pragma once
#include "Console.h"

class ConsoleDlg
{
public:
	static INT_PTR CALLBACK WndProcS(HWND, UINT, WPARAM, LPARAM);
	INT_PTR WndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL Create(HINSTANCE hInst, HWND hParent);
	void Close();
	BOOL IsWindow() const { return ::IsWindow(m_hWnd); }
	HWND GetHandle() const { return m_hWnd; }
private:
	Console m_Console;
	HWND m_hWnd;
	HWND m_hInput;
	HWND m_hOutput;
};

extern ConsoleDlg gConsoleDlg;
