// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>



// TODO:  在此处引用程序需要的其他头文件
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
