#pragma once

class Console
{
public:
	Console();
	virtual ~Console();
	BOOL Init(HWND hWnd);
	void Print(const TCHAR* msg) { InternalPrint(msg, -1); }
	void Print(const TCHAR* msg, int count) { if (count < 0) count = 0; InternalPrint(msg, count); }
private:
	int CheckMemory(int size);
	void NewLine(int new_lines);
	void InternalPrint(const TCHAR* msg, int count);
private:
	HWND m_hWnd;
	int m_lines;
	int m_maxLines;
	TCHAR* m_buff;
	int m_buffCount;
};
