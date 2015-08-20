#include "stdafx.h"
#include "Core.h"
#include "PointLight.h"

PointLightShader gPointLightShader;

struct CBLight
{
	Vector3 color;
	float pad;
	Vector3 position;
	float pad2;
	Vector3 att;
	float pad3;
};

PointLight::PointLight()
	: mIndex(0)
	, mEnable(true)
	, mIntensity(1)
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
		CBLight* dst = (CBLight*)mr.pData;
		Color2Vector3(light->GetColor(), dst->color);
		dst->position = light->GetTransform().position;
		dst->att.x = 0;
		dst->att.y = 0.2f;
		dst->att.z = 0;
		dst->att /= light->GetIntensity();
		context->Unmap(mCB, 0);
	}

	context->PSSetConstantBuffers(1, 1, &mCB);
	context->PSSetShader(mPS, nullptr, 0);
}
