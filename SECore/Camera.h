#pragma once

class Camera : public SECore::Camera
{
public:
	friend class Scene;
public:
	Camera();
	virtual ~Camera();

	virtual void Release();

	virtual const Vector3& GetEye() { return eye; }
	virtual const Vector3& GetLookAt() { return lookat; }
	virtual const Vector3& GetUp() { return up; }

	virtual float GetFov() { return fov; }
	virtual float GetZNear() const { return znear; }
	virtual float GetZFar() { return zfar; }

	virtual void SetEye(const Vector3& v) { eye = v; }
	virtual void SetLookAt(const Vector3& v) { lookat = v; }
	virtual void SetUp(const Vector3& v) { up = v; }

	virtual void ScreenToWorld(const Vector3& src, Vector3& dst);
	virtual void WorldToScreen(const Vector3& src, Vector3& dst);
	virtual void ScreenPointToRay(Ray& ray, const Vector3& point);

	virtual void GetViewProjMatrix(Matrix& dst) const;
	virtual void GetViewport(D3D11_VIEWPORT& dst);

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
