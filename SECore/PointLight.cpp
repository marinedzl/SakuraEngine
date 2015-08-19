#include "stdafx.h"
#include "Core.h"
#include "PointLight.h"

PointLightShader gPointLightShader;

struct CBLight
{
	Vector3 diffuse;
	float diffusePower;

	Vector3 specular;
	float specularPower;

	Vector3 position;
	float specularHardness;

	float range;
	float intensity;
	Vector2 nouse;
};

PointLight::PointLight()
	: mIndex(0)
	, mEnable(true)
{
}

PointLight::~PointLight()
{

}

void PointLight::Setup()
{
	gPointLightShader.Setup(this);
}

PointLightShader::PointLightShader()
{
}

bool PointLightShader::Init()
{
	bool ret = false;

	ID3D11Device* device = gCore.GetDevice();
	buffer file;
	CHECK(LoadBinaryFile(file, "point_light_ps.cso"));
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

void PointLightShader::Release()
{
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mCB);
}

void Color2Vector3(const Color& color, Vector3& dst)
{
	dst.x = color.r;
	dst.y = color.g;
	dst.z = color.b;
}

void PointLightShader::Setup(PointLight * light)
{
	ID3D11DeviceContext* context = gCore.GetContext();

	D3D11_MAPPED_SUBRESOURCE mr;
	if (SUCCEEDED(context->Map(mCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
	{
		const PointLight::Data& src = light->GetData();
		CBLight* dst = (CBLight*)mr.pData;
		Color2Vector3(src.diffuse, dst->diffuse);
		Color2Vector3(src.specular, dst->specular);
		dst->position = light->GetTransform().position;
		dst->diffusePower = src.diffusePower;
		dst->specularPower = src.specularPower;
		dst->specularHardness = src.specularHardness;
		dst->range = src.range;
		dst->intensity = src.intensity;
		context->Unmap(mCB, 0);
	}

	context->PSSetConstantBuffers(1, 1, &mCB);
	context->PSSetShader(mPS, nullptr, 0);
}
