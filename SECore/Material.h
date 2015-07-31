#pragma once
#include "AnyValue.h"

class Material : public IMaterial
{
public:
	Material();
	virtual ~Material();
	virtual void SetShader(IShader* shader);
	virtual void Setup(size_t pass);
	virtual size_t GetPassCount() const { return mPasses.size(); }
	virtual void SetTexture(const char* name, ITexture* value);
public:
	void SetValue(const char* name, const AnyValue& value);
private:
	void Clear();
	void Reload();
private:
	class Pass;
private:
	Shader* mShader;
	array<Pass*> mPasses;
};
