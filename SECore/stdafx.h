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

#include <FreeImage.h>

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

typedef SECore::Shader Shader;
typedef SECore::ShaderFactory ShaderFactory;

class Scene;

class Collider;
class RigidBody;
class CharacterController;
class Mesh;
class Texture;
class Material;
class Renderer;
class Gizmo;
class Skeleton;
class Animation;
class Animator;
class AnimationClip;
class RenderTarget;
class RenderTexture;

class SceneEntity;
class RenderEntity;

class Camera;

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

#define MAX_BONE 256

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

void AffineTransform(XMMATRIX& mat, const Vector3& position, const Quat& rotation, const Vector3& scaling);

inline void AffineTransform(Matrix& mat, const Vector3& position, const Quat& rotation, const Vector3& scaling)
{
	XMMATRIX temp;
	AffineTransform(temp, position, rotation, scaling);
	XMStoreFloat4x4((XMFLOAT4X4*)&mat, temp);
}

inline void AffineTransform(Matrix& mat, const Transform& transform)
{
	AffineTransform(mat, transform.position, transform.rotation, transform.scaling);
}

inline void AffineTransform(XMMATRIX& mat, const Transform& transform)
{
	AffineTransform(mat, transform.position, transform.rotation, transform.scaling);
}

bool MatrixDecompose(const Matrix& mat, Vector3& position, Quat& rotation, Vector3& scaling);

bool MatrixDecompose(const XMMATRIX& mat, Vector3& position, Quat& rotation, Vector3& scaling);

inline bool MatrixDecompose(const Matrix& mat, Transform& transform)
{
	return MatrixDecompose(mat, transform.position, transform.rotation, transform.scaling);
}

inline bool MatrixDecompose(const XMMATRIX& mat, Transform& transform)
{
	return MatrixDecompose(mat, transform.position, transform.rotation, transform.scaling);
}

inline PxVec3 ConvertPxVec3(const Vector3& v)
{
	return PxVec3(v.x, v.y, v.z);
}

inline Vector3 ConvertPxVec3(const PxVec3& v)
{
	return Vector3(v.x, v.y, v.z);
}

inline PxQuat ConvertPxQuat(const Quat& v)
{
	return PxQuat(v.x, v.y, v.z, v.w);
}

inline Quat ConvertPxQuat(const PxQuat& v)
{
	return Quat(v.x, v.y, v.z, v.w);
}

inline void Color2Vector3(const Color& color, Vector3& dst)
{
	dst.x = color.r;
	dst.y = color.g;
	dst.z = color.b;
}

struct GizmoMesh
{
	struct Vertex
	{
		Vector3 pos;
	};
	size_t indexCount;
	D3D_PRIMITIVE_TOPOLOGY primitive;
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	GizmoMesh() : indexCount(0), primitive(D3D11_PRIMITIVE_TOPOLOGY_LINELIST), vb(nullptr), ib(nullptr) {}
};
