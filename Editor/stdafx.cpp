// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// stdafx.cpp : 只包括标准包含文件的源文件
// Editor.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

void log(const char* fmt, ...)
{
	va_list args;
	size_t len;
	char buffer[1024];
	va_start(args, fmt);
	len = vsnprintf_s(buffer, 1024, fmt, args);
	va_end(args);
	printf(buffer);
	OutputDebugStringA(buffer);
}

buffer::buffer()
	: _ptr(nullptr)
	, _size(0)
{

}

buffer::~buffer()
{
	clear();
}

void buffer::clear()
{
	if (_ptr)
	{
		delete _ptr;
		_ptr = nullptr;
	}
	_size = 0;
}

void buffer::resize(size_t size)
{
	clear();
	_size = size;
	_ptr = new char[size];
}

bool LoadDataFromFile(buffer& buff, const TCHAR* filename, bool text)
{
	bool ret = false;
	FILE* file = nullptr;
	size_t size = 0;
	std::string fullpath;

	CHECK(filename);

	buff.clear();

	_tfopen_s(&file, filename, text ? _T("r") : _T("rb"));
	CHECK(file);

	fseek(file, 0L, SEEK_SET);
	fseek(file, 0L, SEEK_END);
	size = ftell(file);

	buff.resize(text ? size + 1 : size);

	fseek(file, 0L, SEEK_SET);
	size = fread(buff.ptr(), 1, size, file);

	if (text)
		buff.ptr()[size] = 0;

	ret = true;
Exit0:
	if (file)
		fclose(file);
	return ret;
}

bool LoadBinaryFile(buffer& buff, const TCHAR* filename)
{
	return LoadDataFromFile(buff, filename, false);
}

bool LoadTextFile(buffer& buff, const TCHAR* filename)
{
	return LoadDataFromFile(buff, filename, true);
}

std::string WStr2MStr(const TCHAR* src)
{
	std::string ret;
	size_t minsize = WideCharToMultiByte(CP_OEMCP, NULL, src, -1, NULL, 0, NULL, FALSE);
	char* buff = new char[minsize];
	CHECK(buff);
	CHECK(WideCharToMultiByte(CP_OEMCP, NULL, src, -1, buff, minsize, NULL, FALSE));
	ret = buff;
Exit0:
	if (buff)
		delete buff;
	return ret;
}

CString MStr2WStr(const char* src)
{
	CString ret;
	size_t minsize = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	wchar_t* buff = new wchar_t[minsize];
	CHECK(buff);
	CHECK(MultiByteToWideChar(CP_ACP, 0, src, -1, buff, minsize));
	ret = buff;
Exit0:
	if (buff)
		delete buff;
	return ret;
}
