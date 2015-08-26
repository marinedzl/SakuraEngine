#include "stdafx.h"
#include "Console.h"

Console::Console()
	: m_hWnd(NULL)
	, m_lines(0)
	, m_maxLines(300)
	, m_buff(NULL)
	, m_buffCount(0)
{
}

Console::~Console()
{
	if (m_buff)
		delete m_buff;
}

BOOL Console::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	return TRUE;
}

int Console::CheckMemory(int count)
{
	int rv = 1;
	if (count + 16 >= m_buffCount)
	{
		int newBuffCount = count + 1024;
		TCHAR* newBuff = (TCHAR*)realloc(m_buff, newBuffCount * sizeof(TCHAR));
		if (newBuff != NULL)
		{
			m_buff = newBuff;
			m_buffCount = newBuffCount;
		}
		else
		{
			rv = 0;
		}
	}
	return rv;
}

void Console::NewLine(int new_lines)
{
	int rem_lines = (new_lines + m_lines + 1) - m_maxLines;
	if (rem_lines <= 0)
	{
		m_lines += new_lines;
	}
	else
	{
		int sel = SendMessage(m_hWnd, EM_LINEINDEX, rem_lines, 0);

		SendMessage(m_hWnd, EM_SETSEL, 0, (LPARAM)sel);
		SendMessage(m_hWnd, EM_REPLACESEL, FALSE, (LPARAM)_T(""));
		SendMessage(m_hWnd, WM_VSCROLL, SB_BOTTOM, NULL);

		m_lines = m_maxLines - 1;
	}
}

void Console::InternalPrint(const TCHAR* msg, int count) {
	int s, t = 0;
	int new_lines = 0;
	TCHAR* tb;

	if (count == -1) count = 0x7fffffff;

	if (msg != NULL && count && CheckMemory(t))
	{
		for (s = 0; msg[s] && (s < count); s++)
		{
			if (msg[s] == _T('\r')) continue;
			if (!CheckMemory(t)) break;
			if (msg[s] == _T('\n'))
			{
				++new_lines;
				m_buff[t++] = _T('\r');
			}
			m_buff[t++] = msg[s];
		}
		m_buff[t] = _T('\0');
	}
	if (t && msg != NULL)
	{
		tb = m_buff;
	}
	else
	{
		++new_lines;
		tb = _T("\r\n");
	}

	SendMessage(m_hWnd, EM_SETSEL, (WPARAM)-2, (LPARAM)-1);
	SendMessage(m_hWnd, EM_REPLACESEL, FALSE, (LPARAM)tb);
	SendMessage(m_hWnd, WM_VSCROLL, SB_BOTTOM, NULL);

	if (new_lines) NewLine(new_lines);
}
