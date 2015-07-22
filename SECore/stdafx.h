// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "Base.h"
#include "Interface.h"

#include <d3d11.h>

typedef SECore::Matrix Matrix;

typedef SECore::Mesh Mesh;
typedef SECore::Texture Texture;
typedef SECore::Shader IShader;
typedef SECore::Material Material;
typedef SECore::Renderer Renderer;
typedef SECore::Animation Animation;

typedef SECore::Scene IScene;


template<class T>
class RefObject : public T
{
public:
	RefObject() : mRefCount(0) {}
	virtual ~RefObject() {}
	virtual void AddRef() { ++mRefCount; }
	virtual void Release() { --mRefCount; if (mRefCount <= 0) delete this; }
private:
	int mRefCount;
};
