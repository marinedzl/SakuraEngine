// stdafx.cpp : 只包括标准包含文件的源文件
// SEExportor.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <io.h>
#include <tchar.h>

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
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
