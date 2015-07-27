// stdafx.cpp : 只包括标准包含文件的源文件
// SEExportor.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

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

bool MByteToWChar(const char* src, wchar_t* dst, size_t size)
{
	bool ret = false;
	size_t minsize = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	CHECK(!(size < minsize));
	CHECK(MultiByteToWideChar(CP_ACP, 0, src, -1, dst, minsize));
	ret = true;
Exit0:
	return ret;
}

bool WCharToMByte(const wchar_t* src, char* dst, size_t size)
{
	bool ret = false;
	size_t minsize = WideCharToMultiByte(CP_OEMCP, NULL, src, -1, NULL, 0, NULL, FALSE);
	CHECK(!(size < minsize));
	WideCharToMultiByte(CP_OEMCP, NULL, src, -1, dst, size, NULL, FALSE);
	ret = true;
Exit0:
	return ret;
}

std::string WStr2MStr(const std::wstring& src)
{
	std::string ret;
	size_t minsize = WideCharToMultiByte(CP_OEMCP, NULL, src.c_str(), -1, NULL, 0, NULL, FALSE);
	char* buff = new char[minsize];
	WideCharToMultiByte(CP_OEMCP, NULL, src.c_str(), -1, buff, minsize, NULL, FALSE);
	ret = buff;
	delete buff;
	return ret;
}
