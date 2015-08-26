#include "stdafx.h"
#include "resource.h"
#include "ConsoleDlg.h"

ConsoleDlg gConsoleDlg;

BOOL CALLBACK ConsoleDlg::WndProcS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return gConsoleDlg.WndProc(hWnd, message, wParam, lParam);
}

BOOL ConsoleDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		m_hInput = GetDlgItem(hWnd, IDC_EDIT_INPUT);
		m_hOutput = GetDlgItem(hWnd, IDC_EDIT_OUTPUT);
		m_Console.Init(m_hOutput);
		return TRUE;

	case WM_COMMAND:
	{
		int highWord HIWORD(wParam);
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_BUTTON_COMMIT:
		{
			size_t count = GetWindowTextLength(m_hInput);
			count += 3;
			TCHAR* buff = new TCHAR[count];
			GetWindowText(m_hInput, buff, count);
			buff[count - 3] = _T('\r');
			buff[count - 2] = _T('\n');
			buff[count - 1] = _T('\0');
			m_Console.Print(buff);
			SetWindowText(m_hInput, _T(""));
			delete buff;
			SetFocus(m_hInput);
		}
		break;
		}
	}
	break;

	case WM_CLOSE:
	{
		Close();
	}
	break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL ConsoleDlg::Create(HINSTANCE hInst, HWND hParent)
{
	if (m_hWnd)
		return FALSE;
	m_hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_CONSOLE), hParent, WndProcS);
	ShowWindow(m_hWnd, SW_SHOW);
	return TRUE;
}

void ConsoleDlg::Close()
{
	if (::IsWindow(m_hWnd))
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}
