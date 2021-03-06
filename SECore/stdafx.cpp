// stdafx.cpp : 只包括标准包含文件的源文件
// SECore.pch 将作为预编译头
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

bool SplitPath::Split(const char* filename)
{
	errno_t err;
	err = _splitpath_s(filename, drive, _MAX_DRIVE, dir, _MAX_DIR, fname,
		_MAX_FNAME, ext, _MAX_EXT);
	return err == 0;
}

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

void AffineTransform(XMMATRIX& mat, const Vector3& position, const Quat& rotation, const Vector3& scaling)
{
	XMVECTOR p = XMLoadFloat3((const XMFLOAT3*)&position);
	XMVECTOR q = XMLoadFloat4((const XMFLOAT4*)&rotation);
	XMVECTOR s = XMLoadFloat3((const XMFLOAT3*)&scaling);
	mat = XMMatrixAffineTransformation(s, XMQuaternionIdentity(), q, p);
}

bool MatrixDecompose(const XMMATRIX& mat, Vector3& position, Quat& rotation, Vector3& scaling)
{
	XMVECTOR p, q, s;
	if (XMMatrixDecompose(&s, &q, &p, mat))
	{
		XMStoreFloat3((XMFLOAT3*)&position, p);
		XMStoreFloat4((XMFLOAT4*)&rotation, q);
		XMStoreFloat3((XMFLOAT3*)&scaling, s);
		return true;
	}
	else
		return false;
}

bool MatrixDecompose(const Matrix& mat, Vector3& position, Quat& rotation, Vector3& scaling)
{
	XMMATRIX m = XMLoadFloat4x4((const XMFLOAT4X4*)&mat);
	return MatrixDecompose(m, position, rotation, scaling);
}
