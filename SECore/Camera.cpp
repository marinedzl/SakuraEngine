#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::ScreenToWorld(const Vector3& src, Vector3& dst)
{
	XMVECTOR v = XMVector3Unproject(XMLoadFloat3((XMFLOAT3*)&src),
		viewportX * rtW, viewportY * rtH, viewportW * rtW, viewportH * rtH, 0, 1, GetProj(), GetView(), XMMatrixIdentity());
	XMStoreFloat3((XMFLOAT3*)&dst, v);
}

void Camera::WorldToScreen(const Vector3& src, Vector3& dst)
{
	XMVECTOR v = XMVector3Project(XMLoadFloat3((XMFLOAT3*)&src),
		viewportX * rtW, viewportY * rtH, viewportW * rtW, viewportH * rtH, 0, 1, GetProj(), GetView(), XMMatrixIdentity());
	XMStoreFloat3((XMFLOAT3*)&dst, v);
}

void Camera::ScreenPointToRay(Ray & ray, const Vector3 & point)
{
	float w = viewportW * rtW;
	float h = viewportH * rtH;

	float px = point.x;
	float py = point.y;

	Vector3 v3Near(px, py, znear);
	Vector3 v3Far(px, py, zfar);

	XMVECTOR vNear = XMVector3Unproject(XMLoadFloat3((XMFLOAT3*)&v3Near),
		viewportX, viewportY, w, h, 0, 1, GetProj(), GetView(), XMMatrixIdentity());

	XMVECTOR vFar = XMVector3Unproject(XMLoadFloat3((XMFLOAT3*)&v3Far),
		viewportX, viewportY, w, h, 0, 1, GetProj(), GetView(), XMMatrixIdentity());

	XMVECTOR vDir = vNear - vFar;
	vDir = XMVector3Normalize(vDir);

	XMStoreFloat3((XMFLOAT3*)&ray.origin, vNear);
	XMStoreFloat3((XMFLOAT3*)&ray.direction, vDir);
}

void Camera::GetViewProjMatrix(Matrix& dst) const
{
	XMStoreFloat4x4((XMFLOAT4X4*)&dst, GetView() * GetProj());
}

void Camera::GetViewport(D3D11_VIEWPORT& dst)
{
	dst.TopLeftX = viewportX * rtW;
	dst.TopLeftY = viewportY * rtH;
	dst.Width = viewportW * rtW;
	dst.Height = viewportH * rtH;
	dst.MinDepth = 0;
	dst.MaxDepth = 1.0f;
}

XMMATRIX Camera::GetProj() const
{
	XMMATRIX proj;
	if (projectType == Camera::Perspective)
		proj = XMMatrixPerspectiveFovLH(fov, rtW / rtH, znear, zfar);
	else
		proj = XMMatrixOrthographicLH(viewW, viewH, znear, zfar);
	return proj;
}

XMMATRIX Camera::GetView() const
{
	return XMMatrixLookAtLH(XMLoadFloat3(eye), XMLoadFloat3(lookat), XMLoadFloat3(up));
}
