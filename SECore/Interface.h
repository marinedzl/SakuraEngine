#pragma once

namespace SECore
{
	struct Vector2
	{
		float x, y;
		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2 operator+(const Vector2& other)
		{
			return Vector2(this->x + other.x, this->y + other.y);
		}
		Vector2 operator-(const Vector2& other)
		{
			return Vector2(this->x - other.x, this->y - other.y);
		}
		Vector2 operator*(const Vector2& other)
		{
			return Vector2(this->x * other.x, this->y * other.y);
		}
	};

	inline Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x + b.x, a.y + b.y);
	}

	inline Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x - b.x, a.y - b.y);
	}

	inline Vector2 operator*(const Vector2& a, float b)
	{
		return Vector2(a.x * b, a.y * b);
	}

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
		Vector3& operator*=(float other)
		{
			this->x *= other;
			this->y *= other;
			this->z *= other;
			return *this;
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

	inline Vector3 operator*(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	inline Vector3 operator/(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	inline Vector3 operator*(const Vector3& a, float b)
	{
		return Vector3(a.x * b, a.y * b, a.z * b);
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
	};

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

	struct Transform 
	{
		Vector3 position;
		Quat rotation;
		Vector3 scaling;
		Transform() : position(0, 0, 0), rotation(0, 0, 0, 1), scaling(1, 1, 1) {}
	};

	struct Ray
	{
		Vector3 origin;
		Vector3 direction;
	};

	struct RaycastHit;

	struct Object
	{
		virtual ~Object() {}
		virtual void Release() = 0;
	};

	struct RefObject : Object
	{
		virtual ~RefObject() {}
		virtual void AddRef() = 0;
	};

	struct Mesh : RefObject
	{
		virtual ~Mesh() {}
	};

	struct Texture : RefObject
	{
		virtual ~Texture() {}
		virtual void SetSlot(int slot) = 0;
	};

	struct Shader : Object
	{
		enum PropertyType
		{
			eInvalid,
			eFloat,
			eColor,
			eTexture,
		};
		virtual ~Shader() {}
		virtual bool Init() = 0;
		virtual size_t GetPassCount() const = 0;
		virtual bool SetPass(size_t pass) const = 0;
		virtual bool SetValue(const char* name, const void* data) = 0;
		virtual bool SetTexture(const char* name, Texture* texture) = 0;
		virtual PropertyType GetPropertyType(const char* name) const = 0;
	};

	struct ShaderFactory : Object 
	{
		virtual ~ShaderFactory() {}
		virtual Shader* CreateNew() = 0;
	};

	struct Material
	{
		virtual ~Material() {}
		virtual size_t GetPassCount() const = 0;
		virtual bool SetPass(size_t pass) const = 0;
		virtual void SetShader(Shader* shader) = 0;
		virtual void SetFloat(const char* name, float value) = 0;
		virtual void SetColor(const char* name, const Color& color) = 0;
		virtual void SetTexture(const char* name, Texture* texture) = 0;
	};

	struct Animation
	{
		typedef void ClipChangedCallback(void* data, const char* prev, const char* curr);
		virtual ~Animation() {}
		virtual void Play(const char* name, bool loop = true) = 0;
		virtual void CrossFade(const char* name, float fadeLength, bool loop = true) = 0;
		virtual void CrossFadeQueue(const char* name, float offset, float fadeLength, bool loop = true) = 0;
		virtual void SetClipChangedCallback(ClipChangedCallback* callback, void* userData) = 0;
		virtual bool GetSavedBoneTM(const char* bone, Matrix& dst) const = 0;
	};

	struct CharacterController
	{
		virtual ~CharacterController() {}
		virtual bool Move(const Vector3 & offset, float elapsedTime) = 0;
	};

	struct RigidBody
	{
		virtual ~RigidBody() {}
		virtual void SetMass(float mass) = 0;
		virtual void EnableGravity(bool enable) = 0;
		virtual void SetLinearVelocity(const Vector3& linVel) = 0;
		virtual void SetAngularVelocity(const Vector3& angVel) = 0;
	};

	struct Collider
	{
		virtual ~Collider() {}
		virtual RigidBody* GetRigidBody() = 0;
		virtual void SetLocalPose(const Vector3& pos, const Quat& rot) = 0;
	};

	struct Renderer
	{
		struct Entity
		{
			virtual ~Entity() {}
			virtual Mesh* GetMesh() = 0;
			virtual Material* GetMaterial() = 0;
			virtual void SetMesh(Mesh* mesh) = 0;
			virtual const Transform& GetTransform() const = 0;
			virtual bool IsSkinned() const = 0;
		};

		virtual ~Renderer() {}
		virtual void ClearEntities() = 0;
		virtual Entity* CreateEntity() = 0;
		virtual size_t GetEntityCount() = 0;
		virtual Entity* GetEntity(size_t index) = 0;
	};

	struct RenderTarget : Object
	{
		virtual ~RenderTarget() {}
		virtual bool Begin() = 0;
		virtual void End() = 0;
		virtual void Clear(const Color& color) = 0;
		virtual float GetWidth() const = 0;
		virtual float GetHeight() const = 0;
		virtual void ClearDepth() = 0;
	};

	struct Camera
	{
		enum ProjectType
		{
			Perspective,
			Orthographic,
		} projectType;

		virtual ~Camera() {}

		virtual const Vector3& GetEye() = 0;
		virtual const Vector3& GetLookAt() = 0;
		virtual const Vector3& GetUp() = 0;

		virtual float GetFov() = 0;
		virtual float GetZNear() const = 0;
		virtual float GetZFar() = 0;

		virtual void SetEye(const Vector3& v) = 0;
		virtual void SetLookAt(const Vector3& v) = 0;
		virtual void SetUp(const Vector3& v) = 0;

		virtual void ScreenToWorld(const Vector3& src, Vector3& dst) = 0;
		virtual void WorldToScreen(const Vector3& src, Vector3& dst) = 0;
		virtual void ScreenPointToRay(Ray& ray, const Vector3& point) = 0;
	};

	struct Light
	{
		struct Data
		{
			Color diffuse;
			Color specular;
			float diffusePower;
			float specularPower;
			float specularHardness;
			Data()
				: diffusePower(10)
				, specularPower(10)
				, specularHardness(80)
			{
			}
		};
		virtual ~Light() {}
		virtual bool IsEnable() const = 0;
		virtual void Enable(bool enable) = 0;
		virtual Data& GetData() = 0;
		virtual const Data& GetData() const = 0;
		virtual void SetName(const char* name) = 0;
		virtual const char* GetName() const = 0;
		virtual Transform& GetTransform() = 0;
		virtual const Transform& GetTransform() const = 0;
		virtual void Setup() = 0;
	};

	struct PointLight : Light
	{
		virtual ~PointLight() {}
	};

	struct Scene : public Object
	{
		struct Config
		{
			virtual ~Config() {}
			virtual void EnableGizmo(bool enable) = 0;
		};
		struct Entity
		{
			virtual ~Entity() {}

			virtual void SetName(const char* name) = 0;
			virtual const char* GetName() const = 0;

			virtual void DestroyRenderer() = 0;
			virtual Renderer* GetRenderer() = 0;
			virtual Renderer* CreateRenderer() = 0;

			virtual void DestroyAnimation() = 0;
			virtual Animation* GetAnimation() = 0;
			virtual Animation* CreateAnimation() = 0;

			virtual void DestroyCollider() = 0;
			virtual Collider* GetCollider() = 0;
			virtual Collider* CreateBoxCollider(bool isDynamic, const Vector3& size) = 0;

			virtual void DestroyCCT() = 0;
			virtual CharacterController* GetCCT() = 0;
			virtual CharacterController* CreateCCT(float height, float radius, const Vector3& offset) = 0;

			virtual void Update(float deltaTime) = 0;

			virtual Transform& GetTransform() = 0;
			virtual const Transform& GetTransform() const = 0;
		};

		virtual ~Scene() {}

		virtual Camera* GetCamera() = 0;

		virtual void Draw(RenderTarget* rt) = 0;
		virtual void Update(float deltaTime) = 0;

		virtual void ClearEntities() = 0;
		virtual Entity* CreateEntity() = 0;
		virtual Entity* FindEntity(const char* name) = 0;
		virtual void RemoveEntity(Entity* entity) = 0;

		virtual Light* FindLight(const char* name) = 0;

		virtual bool LoadAdditive(const char* filename) = 0;

		virtual bool Raycast(const Ray& ray, RaycastHit& hit, float distance) = 0;

		virtual Config* GetConfig() = 0;
	};

	struct RaycastHit
	{
		Vector3 point;
		Vector3 normal;
		float distance;
		const Scene::Entity* entity;
	};
}
