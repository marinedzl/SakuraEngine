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
