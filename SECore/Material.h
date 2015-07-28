#pragma once

class Material : public IMaterial
{
public:
	Material();
	virtual ~Material();
	virtual void SetShader(IShader* shader);
	virtual void Setup(size_t pass);
	virtual size_t GetPassCount() const { return mPasses.size(); }
	virtual void SetFloat(const char* name, float value);
	virtual void SetColor(const char* name, const Color& value);
	virtual void SetTexture(const char* name, ITexture* value);
private:
	void Clear();
	void Reload();
private:
	void SetValue(const char* name, const char* data, int type, size_t size);
private:
	class Pass;
private:
	Shader* mShader;
	array<Pass*> mPasses;
};
