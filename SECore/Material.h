#pragma once

class MaterialImpl : public Material
{
public:
	MaterialImpl();
	virtual ~MaterialImpl();
	virtual void SetShader(Shader* shader);
	virtual void SetFloat(const char* name, float value);
	virtual void SetColor(const char* name, const Color& value);
	virtual void SetTexture(const char* name, ITexture* texture);
	virtual size_t GetPassCount() const { return mShader->GetPassCount(); }
	virtual bool SetPass(size_t pass) const { return mShader->SetPass(pass); }
private:
	Shader* mShader;
};
