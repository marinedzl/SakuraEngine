// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// SEExportor.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include <io.h>
#include <tchar.h>

// TODO: �� STDAFX.H �������κ�����ĸ���ͷ�ļ���
//�������ڴ��ļ�������
void log(const TCHAR* fmt, ...)
{
	va_list args;
	size_t len;
	TCHAR buffer[1024];
	va_start(args, fmt);
	len = _vsnwprintf_s(buffer, 1024, fmt, args);
	va_end(args);
	wprintf(buffer);
	OutputDebugString(buffer);
}
