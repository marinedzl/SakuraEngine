// stdafx.cpp : 只包括标准包含文件的源文件
// Test.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

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

bool LoadDataFromFile(buffer& buff, const char* filename, bool text)
{
	bool ret = false;
	FILE* file = nullptr;
	size_t size = 0;
	std::string fullpath;

	CHECK(filename);

	buff.clear();

	fopen_s(&file, filename, text ? "r" : "rb");
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

bool LoadBinaryFile(buffer& buff, const char* filename)
{
	return LoadDataFromFile(buff, filename, false);
}

bool LoadTextFile(buffer& buff, const char* filename)
{
	return LoadDataFromFile(buff, filename, true);
}
