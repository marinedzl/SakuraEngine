#pragma once

template<class T>
class BaseLight : public T
{
public:
	BaseLight() : mEnable(true), mIntensity(1) {}
	virtual ~BaseLight() {}
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
	virtual void GetVP(Matrix& vp) const {}
private:
	std::string mName;
	Transform mTransform;
	bool mEnable;
	float mIntensity;
	Color mColor;
};
