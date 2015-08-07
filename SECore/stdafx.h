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
#include "PxPhysicsAPI.h"
using namespace physx;

void log(const TCHAR* fmt, ...);

typedef SECore::Matrix Matrix;
typedef SECore::Color Color;
typedef SECore::Vector2 Vector2;
typedef SECore::Vector3 Vector3;
typedef SECore::Vector4 Vector4;
typedef SECore::Quat Quat;
typedef SECore::Transform Transform;
typedef SECore::Ray Ray;
typedef SECore::RaycastHit RaycastHit;

typedef SECore::Object Object;
typedef SECore::RefObject RefObject;

typedef SECore::Collider Collider;
typedef SECore::Shader Shader;
typedef SECore::ShaderFactory ShaderFactory;

class Scene;

class Mesh;
class Texture;
class Material;
class Renderer;
class Skeleton;
class Animation;
class AnimationClip;

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

inline void MatrixMul(Matrix& dst, const Matrix& m0, const Matrix& m1)
{
	XMStoreFloat4x4((XMFLOAT4X4*)&dst, XMLoadFloat4x4((const XMFLOAT4X4*)&m0) * XMLoadFloat4x4((const XMFLOAT4X4*)&m1));
}

inline XMVECTOR XMLoadFloat3(const Vector3& src)
{
	return XMLoadFloat3((const XMFLOAT3*)&src);
}

inline XMVECTOR XMLoadFloat4(const Vector4& src)
{
	return XMLoadFloat4((const XMFLOAT4*)&src);
}

inline XMVECTOR XMLoadFloat4(const Quat& src)
{
	return XMLoadFloat4((const XMFLOAT4*)&src);
}

inline void XMStoreFloat4x4(Matrix& dst, const XMMATRIX& M)
{
	XMStoreFloat4x4((XMFLOAT4X4*)&dst, M);
}

void AffineTransform(Matrix& mat, const Vector3& position, const Quat& rotation, const Vector3& scaling);
inline void AffineTransform(Matrix& mat, const Transform& transform)
{
	AffineTransform(mat, transform.position, transform.rotation, transform.scaling);
}
bool MatrixDecompose(const Matrix& mat, Vector3& position, Quat& rotation, Vector3& scaling);

inline PxVec3 ConvertPxVec3(const Vector3& v)
{
	return PxVec3(v.x, v.y, v.z);
}

inline Vector3 ConvertPxVec3(const PxVec3& v)
{
	return Vector3(v.x, v.y, v.z);
}

inline PxQuat GetPxQuat(const Quat& v)
{
	return PxQuat(v.x, v.y, v.z, v.w);
}
