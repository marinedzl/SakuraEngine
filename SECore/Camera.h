#pragma once

class Camera : public SECore::Camera
{
public:
	friend class Scene;
public:
	Camera();
	virtual ~Camera();

	virtual void Release();

	virtual const Vector3& GetEye() const { return eye; }
	virtual const Vector3& GetLookAt() const { return lookat; }
	virtual const Vector3& GetUp() const { return up; }

	virtual float GetFov() const { return fov; }
	virtual float GetZNear() const { return znear; }
	virtual float GetZFar() const { return zfar; }

	virtual void SetEye(const Vector3& v) { eye = v; }
	virtual void SetLookAt(const Vector3& v) { lookat = v; }
	virtual void SetUp(const Vector3& v) { up = v; }
	virtual void SetView(float w, float h) { rtW = w; rtH = h; }

	virtual void ScreenToWorld(const Vector3& src, Vector3& dst) const;
	virtual void WorldToScreen(const Vector3& src, Vector3& dst) const;
	virtual void ScreenPointToRay(Ray& ray, const Vector3& point) const;

	virtual void GetViewProjMatrix(Matrix& dst) const;
	virtual void GetViewport(D3D11_VIEWPORT& dst) const;

public:
	XMMATRIX GetProj() const;
	XMMATRIX GetView() const;

private:
	Vector3 eye;
	Vector3 lookat;
	Vector3 up;

	float fov;
	float znear;
	float zfar;

	float rtW;
	float rtH;

	float viewW;
	float viewH;

	float viewportX;
	float viewportY;
	float viewportW;
	float viewportH;
};
