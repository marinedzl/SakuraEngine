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
		viewportX, viewportY, viewportW, viewportH, 0, 1, GetProj(), GetView(), XMMatrixIdentity());
	XMStoreFloat3((XMFLOAT3*)&dst, v);
}

void Camera::WorldToScreen(const Vector3& src, Vector3& dst)
{
	XMVECTOR v = XMVector3Project(XMLoadFloat3((XMFLOAT3*)&src),
		viewportX, viewportY, viewportW, viewportH, 0, 1, GetProj(), GetView(), XMMatrixIdentity());
	XMStoreFloat3((XMFLOAT3*)&dst, v);
}

void Camera::GetViewProjMatrix(Matrix& dst)
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

XMMATRIX Camera::GetProj()
{
	XMMATRIX proj;
	if (projectType == Camera::Perspective)
		proj = XMMatrixPerspectiveFovLH(fov, rtW / rtH, znear, zfar);
	else
		proj = XMMatrixOrthographicLH(viewW, viewH, znear, zfar);
	return proj;
}

XMMATRIX Camera::GetView()
{
	return XMMatrixLookAtLH(XMLoadFloat3(eye), XMLoadFloat3(lookat), XMLoadFloat3(up));
}
