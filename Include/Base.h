#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>

#ifdef _DEBUG
#include <crtdbg.h>
#define CHECKM(expr, ...) \
	if (!(expr)) { \
		if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, __VA_ARGS__)) \
			_CrtDbgBreak(); \
		goto Exit0; \
															}
#define _EnableAllocDbg() _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF)
#else
#define CHECKM(expr, ...) \
	if (!(expr)) { \
		goto Exit0; \
			}
#define _EnableAllocDbg()
#endif

#define CHECK(expr) CHECKM(expr, "%s", #expr)

void log(const char* fmt, ...);

template<typename T>
void SAFE_DELETE(T*& p)
{
	if (p)
		delete p;
	p = nullptr;
}

template<typename T>
void SAFE_RELEASE(T*& p)
{
	if (p)
		p->Release();
	p = nullptr;
}

template<typename T>
void ReleaseList(T& v)
{
	if (!v.empty())
	{
		T::iterator iter = v.begin();
		T::iterator iterEnd = v.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(*iter);
		}
		v.clear();
	}
}

template<typename T>
void ReleaseMap(T& v)
{
	if (!v.empty())
	{
		T::iterator iter = v.begin();
		T::iterator iterEnd = v.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
		v.clear();
	}
}

template<typename T>
void DeleteArray(T& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		SAFE_DELETE(v[i]);
	}
	v.clear();
}

template<typename T>
void ReleaseArray(T& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		SAFE_RELEASE(v[i]);
	}
	v.clear();
}

template<typename T>
void DeleteList(T& v)
{
	if (!v.empty())
	{
		T::iterator iter = v.begin();
		T::iterator iterEnd = v.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(*iter);
		}
		v.clear();
	}
}

template<typename T>
void DeleteMap(T& v)
{
	if (!v.empty())
	{
		T::iterator iter = v.begin();
		T::iterator iterEnd = v.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		v.clear();
	}
}

template<typename T>
class array
{
public:
	array() {}
	~array() { clear(); }
	size_t size() const { return _count; }
	T& at(size_t index) const { return _elements[index]; }
	void clear() { delete _elements; _elements = nullptr; }
	void resize(size_t count) { clear(); this->_count = count; _elements = new T[count]; }
	T& operator [](size_t index) { return _elements[index]; }
	const T& operator [](size_t index) const { return _elements[index]; }
	T* ptr() { return _elements; }
	const T* ptr() const { return _elements; }
private:
	size_t _count;
	T* _elements;
};
