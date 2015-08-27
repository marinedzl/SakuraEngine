#pragma once
#include "BaseLight.h"

class PointLight : public BaseLight<SECore::PointLight>
{
public:
	PointLight();
	virtual ~PointLight();
	virtual void Setup();
private:
};

class PointLightShader
{
public:
	PointLightShader();
	bool Init();
	void Release();
	void Setup(PointLight* light);
private:
	ID3D11Buffer* mCB;
	ID3D11PixelShader* mPS;
};

extern PointLightShader gPointLightShader;
