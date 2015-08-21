#include "stdafx.h"
#include "Core.h"
#include "DirectionalLight.h"

DirectionalLightShader gDirectionalLightShader;

struct CBLight
{
	Vector3 color;
	float att;
	Vector3 dir;
	float pad;
};

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::Setup()
{
	gDirectionalLightShader.Setup(this);
}

DirectionalLightShader::DirectionalLightShader()
{
}

bool DirectionalLightShader::Init()
{
	bool ret = false;

	ID3D11Device* device = gCore.GetDevice();
	buffer file;
	CHECK(LoadBinaryFile(file, "directional_light.cso"));
	CHECK(SUCCEEDED(device->CreatePixelShader(file.ptr(), file.size(), nullptr, &mPS)));

	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(buffDesc));
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.ByteWidth = sizeof(CBLight);
	CHECK(SUCCEEDED(device->CreateBuffer(&buffDesc, nullptr, &mCB)));

	ret = true;
Exit0:
	return ret;
}

void DirectionalLightShader::Release()
{
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mCB);
}

void DirectionalLightShader::Setup(DirectionalLight * light)
{
	ID3D11DeviceContext* context = gCore.GetContext();

	D3D11_MAPPED_SUBRESOURCE mr;
	if (SUCCEEDED(context->Map(mCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
	{
		CBLight* dst = (CBLight*)mr.pData;
		Color2Vector3(light->GetColor(), dst->color);
		dst->dir = light->GetTransform().position;
		dst->att = light->GetIntensity();
		context->Unmap(mCB, 0);
	}

	context->PSSetConstantBuffers(1, 1, &mCB);
	context->PSSetShader(mPS, nullptr, 0);
}
