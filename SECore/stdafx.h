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
#include <DirectXMath.h>
using namespace DirectX;
#include <json\json.h>

typedef SECore::Matrix Matrix;
typedef SECore::Color Color;
typedef SECore::Vector2 Vector2;
typedef SECore::Vector3 Vector3;
typedef SECore::Vector4 Vector4;
typedef SECore::Quat Quat;

typedef SECore::Object Object;
typedef SECore::RefObject RefObject;

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
class TRefObject : public T
{
public:
	TRefObject() : mRefCount(0) {}
	virtual ~TRefObject() {}
	virtual void AddRef() { ++mRefCount; }
	virtual void Release() { --mRefCount; }
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

bool LoadBinaryFile(buffer& buff, const char* filename);
bool LoadTextFile(buffer& buff, const char* filename);

struct SplitPath
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	bool Split(const char* filename);

	std::string GetPath() const
	{
		std::string ret = drive;
		ret += dir;
		if (ret.empty())
		{
			ret = ".";
		}
		return ret;
	}
};

void AffineTransform(Matrix& mat, const Vector3& position, const Quat& rotation, const Vector3& scaling);
bool MatrixDecompose(const Matrix& mat, Vector3& position, Quat& rotation, Vector3& scaling);
