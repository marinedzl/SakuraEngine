#include "stdafx.h"
#include "Utility.h"

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

std::wstring MStr2WStr(const std::string& src)
{
	std::wstring ret;
	size_t minsize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	wchar_t* buff = new wchar_t[minsize];
	CHECK(MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, buff, minsize));
	ret = buff;
	delete buff;
Exit0:
	return ret;
}

bool IsPathExist(const TCHAR* path)
{
	return _taccess(path, 0) != -1;
}

inline TCHAR* strrchrm(TCHAR* _str, const TCHAR* _cmp)
{
	TCHAR* _end = _str + _tcslen(_str) - 1;
	while (_end != _str)
	{
		const TCHAR* _ch = _cmp;
		while (*_ch != _T('\0'))
		{
			if (*_ch == *_end)
			{
				return _end;
			}
			++_ch;
		}
		--_end;
	}
	return NULL;
}

bool CreateMultiDir(const TCHAR* lpDir)
{
	if (!lpDir || *lpDir == '\0')
		return false;

	if (IsPathExist(lpDir))
		return true;

	if (!CreateDirectory(lpDir, NULL))
	{
		WCHAR szParentDir[MAX_PATH] = { 0 };
		_tcscpy_s(szParentDir, MAX_PATH, lpDir);
		TCHAR * pEnd = strrchrm(szParentDir, _T("/\\"));
		if (!pEnd)
			return false;
		*pEnd = _T('\0');
		CreateMultiDir(szParentDir);
		CreateDirectory(lpDir, NULL);
	}

	return true;
}

namespace MaxPlugin
{
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

		buff.resize(text ? size + sizeof(char) : size);

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

	bool LoadJsonFromFile(const TCHAR* filename, Json::Value& root)
	{
		bool ret = false;
		buffer file;
		Json::Reader reader;

		CHECK(LoadTextFile(file, filename) && file.size() > 0);

		if (!reader.parse(file.ptr(), root))
		{
			//log(reader.getFormatedErrorMessages().c_str());
			CHECK(false);
		}

		ret = true;
	Exit0:
		return ret;
	}

	bool SaveJsonToFile(const TCHAR* filename, const Json::Value& root)
	{
		bool ret = false;
		FILE* file = nullptr;
		Json::StyledWriter writer;
		std::string dst;

		dst = writer.write(root);
		_tfopen_s(&file, filename, _T("w"));
		CHECK(file);
		fwrite(dst.c_str(), 1, dst.length(), file);

		ret = true;
	Exit0:
		if (file)
			fclose(file);
		return ret;
	}
}
