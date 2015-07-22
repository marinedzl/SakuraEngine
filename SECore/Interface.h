#pragma once

namespace SECore
{
	/*
	struct Vector3
	{
		float x, y, z;
		Vector3() : x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3 operator+(const Vector3& other)
		{
			return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
		}
		Vector3 operator-(const Vector3& other)
		{
			return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
		}
	};

	inline Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	inline Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	struct Vector4
	{
		float x, y, z, w;
		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	};

	struct Quat
	{
		float x, y, z, w;
		Quat() : x(0), y(0), z(0), w(1) {}
		Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	};

	struct Color
	{
		float r, g, b, a;
		Color() : r(1), g(1), b(1), a(1) {}
		Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	};*/

	struct Matrix
	{
		union
		{
			struct
			{
				float m[4][4];
			};
			struct
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};
		};

		Matrix()
			: m00(1), m01(0), m02(0), m03(0)
			, m10(0), m11(1), m12(0), m13(0)
			, m20(0), m21(0), m22(1), m23(0)
			, m30(0), m31(0), m32(0), m33(1)
		{}
	};

	struct Object
	{
		virtual void Release() = 0;
	};

	struct RefObject : Object
	{
		virtual void AddRef() = 0;
	};

	struct Mesh : RefObject
	{

	};

	struct Texture : RefObject
	{

	};

	struct Shader : RefObject
	{
	};

	struct Material : RefObject
	{

	};

	struct Animation
	{
		virtual void Play(const char* clipname) = 0;
		virtual bool GetMatrix(Matrix* dst, size_t count) const = 0;
	};

	struct Renderer
	{
		struct Entity
		{
			virtual Mesh* GetMesh() = 0;
			virtual Material* GetMaterial() = 0;
			virtual void SetMesh(Mesh* mesh) = 0;
			virtual void SetMaterial(Material* material) = 0;
			virtual const Matrix& GetWorld() const = 0;
			virtual void SetWorld(const Matrix& m) = 0;
		};

		virtual void ClearEntities() = 0;
		virtual Entity* CreateEntity() = 0;
		virtual size_t GetEntityCount() = 0;
		virtual Entity* GetEntity(size_t index) = 0;
	};

	struct Scene : public Object
	{
		struct Entity
		{
			virtual void DestroyRenderer() = 0;
			virtual Renderer* GetRenderer() = 0;
			virtual Renderer* CreateRenderer() = 0;

			virtual void DestroyAnimation() = 0;
			virtual Animation* GetAnimation() = 0;
			virtual Animation* CreateAnimation() = 0;

			virtual void Update(float deltaTime) = 0;

			virtual const Matrix& GetWorld() const = 0;
			virtual void SetWorld(const Matrix& m) = 0;
		};

		virtual void Draw() = 0;
		virtual void Update(float deltaTime) = 0;

		virtual void ClearEntities() = 0;
		virtual Entity* CreateEntity() = 0;
		virtual void RemoveEntity(Entity* entity) = 0;
	};
}
