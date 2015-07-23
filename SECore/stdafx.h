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
typedef SECore::Color Color;
typedef SECore::Vector3 Vector3;

typedef SECore::Object Object;

typedef SECore::Scene IScene;
typedef SECore::RenderTarget IRenderTarget;

typedef SECore::Mesh IMesh;
typedef SECore::Shader IShader;
typedef SECore::Texture ITexture;
typedef SECore::Material IMaterial;
typedef SECore::Renderer IRenderer;
typedef SECore::Animation IAnimation;

class Scene;

class Mesh;
class Shader;
class Texture;
class Material;
class Renderer;
class Animation;

class SceneEntity;
class RenderEntity;

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


enum AlphaTestMode
{
	AlphaTestLess,
	AlphaTestGreater,
	AlphaTestLEqual,
	AlphaTestGEqual,
	AlphaTestEqual,
	AlphaTestNotEqual,
	AlphaTestAlways,
};

enum CBSlot
{
	eCBGlobal,
	eCBModel,
	eCBProperty,
	eCBCount,
};

struct CBGlobal
{
	enum { slot = eCBGlobal };
	Matrix MATRIX_VP;
};

#define MAX_BONE 256

struct CBModel
{
	enum { slot = eCBModel };
	Matrix MATRIX_M;
	Matrix MATRIX_M_SKIN[MAX_BONE];
};

#define texture_slot_reserve 2
