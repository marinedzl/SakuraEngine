#pragma once

class PointLight : public SECore::PointLight
{
public:
	PointLight();
	virtual ~PointLight();
	virtual void SetName(const char* name) { mName = name; }
	virtual const char* GetName() const { return mName.c_str(); }
	virtual Transform& GetTransform() { return mTransform; }
	virtual const Transform& GetTransform() const { return mTransform; }
	virtual bool IsEnable() const { return mEnable; }
	virtual void Enable(bool enable) { mEnable = enable; }
	virtual float GetIntensity() const { return mIntensity; }
	virtual void SetIntensity(float intensity) { mIntensity = intensity; }
	virtual const Color& GetColor() const { return mColor; }
	virtual void SetColor(const Color& color) { mColor = color; }
	virtual void Setup();
private:
	std::string mName;
	Transform mTransform;
	UINT mIndex;
	bool mEnable;
	float mIntensity;
	Color mColor;
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
