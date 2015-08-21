#pragma once
#include "BaseLight.h"

class DirectionalLight : public BaseLight<SECore::PointLight>
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();
	virtual void Setup();
private:
};

class DirectionalLightShader
{
public:
	DirectionalLightShader();
	bool Init();
	void Release();
	void Setup(DirectionalLight* light);
private:
	ID3D11Buffer* mCB;
	ID3D11PixelShader* mPS;
};

extern DirectionalLightShader gDirectionalLightShader;
