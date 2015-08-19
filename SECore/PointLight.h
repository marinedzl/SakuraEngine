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
	virtual Data& GetData() { return mData; }
	virtual const Data& GetData() const { return mData; }
	virtual void Setup();
private:
	std::string mName;
	Transform mTransform;
	UINT mIndex;
	bool mEnable;
	Data mData;
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
