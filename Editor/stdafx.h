
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include "../SECore/SECore.h"
#include <json\json.h>
#include <DirectXMath.h>
using namespace DirectX;

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

typedef SECore::Color Color;
typedef SECore::Vector2 Vector2;
typedef SECore::Vector3 Vector3;
typedef SECore::Vector4 Vector4;
typedef SECore::Quat Quat;
typedef SECore::Matrix Matrix;
typedef SECore::Texture Texture;
typedef SECore::Camera Camera;
typedef SECore::Scene::Entity GameObject;
typedef SECore::Light Light;
typedef SECore::Animation Animation;
typedef SECore::Transform Transform;
typedef SECore::Ray Ray;
typedef SECore::RaycastHit RaycastHit;
typedef SECore::RigidBody RigidBody;
typedef SECore::CharacterController CharacterController;

class buffer
{
public:
	buffer();
	~buffer();
	void clear();
	void resize(size_t size);
	char* ptr() { return _ptr; }
	const char* ptr() const { return _ptr; }
	size_t size() const { return _size; }
private:
	char* _ptr;
	size_t _size;
};

bool LoadBinaryFile(buffer& buff, const TCHAR* filename);
bool LoadTextFile(buffer& buff, const TCHAR* filename);

std::string WStr2MStr(const TCHAR* src);
CString MStr2WStr(const char* src);
