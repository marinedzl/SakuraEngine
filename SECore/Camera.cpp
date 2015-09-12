#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	projectType = Camera::Perspective;
	eye = Vector3(0, 0, -10);
	lookat = Vector3(0, 0, 0);
	up = Vector3(0, 1, 0);
	fov = XM_PI / 4;
	znear = 0.1f;
	zfar = 1000.0f;
	viewportX = 0;
	viewportY = 0;
	viewportW = 1;
	viewportH = 1;
}

Camera::~Camera()
{
}

void Camera::Release()
{
	delete this;
}

void Camera::ScreenToWorld(const Vector3& src, Vector3& dst) const
{
	XMVECTOR v = XMVector3Unproject(XMLoadFloat3((XMFLOAT3*)&src),
		viewportX * rtW, viewportY * rtH, viewportW * rtW, viewportH * rtH, 0, 1, GetProj(), GetView(), XMMatrixIdentity());
	XMStoreFloat3((XMFLOAT3*)&dst, v);
}

void Camera::WorldToScreen(const Vector3& src, Vector3& dst) const
{
	XMVECTOR v = XMVector3Project(XMLoadFloat3((XMFLOAT3*)&src),
		viewportX * rtW, viewportY * rtH, viewportW * rtW, viewportH * rtH, 0, 1, GetProj(), GetView(), XMMatrixIdentity());
	XMStoreFloat3((XMFLOAT3*)&dst, v);
}

void Camera::ScreenPointToRay(Ray & ray, const Vector3 & point) const
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

void Camera::GetViewport(D3D11_VIEWPORT& dst) const
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
	{
		float distance = 0;
		XMVECTOR v = XMVector3Length(XMLoadFloat3(lookat - eye));
		XMStoreFloat(&distance, v);
		proj = XMMatrixOrthographicLH(rtW / rtH * distance, 1 * distance, znear, zfar);
	}
	return proj;
}

XMMATRIX Camera::GetView() const
{
	return XMMatrixLookAtLH(XMLoadFloat3(eye), XMLoadFloat3(lookat), XMLoadFloat3(up));
}
