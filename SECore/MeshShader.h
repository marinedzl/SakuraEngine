#pragma once

class MeshShader : public Shader
{
public:
	struct CBPS
	{
		Color color;
		float cutoff;
		float nouse[3];
	};
public:
	virtual ~MeshShader();
	virtual bool Init();
	virtual void Release() { delete this; }
	virtual bool SetPass(size_t pass) const;
	virtual size_t GetPassCount() const { return 1; }
	virtual bool SetValue(const char* name, const void* data);
	virtual bool SetTexture(const char* name, SECore::Texture* texture);
	virtual PropertyType GetPropertyType(const char* name) const;
public:
	MeshShader();
private:
	CBPS mPSBuffer;
	ID3D11Buffer* mCBPS;
	Texture* mMainTexture;
	ID3D11PixelShader* mPS;
	bool isDirty;
};

struct MeshShaderFactory : ShaderFactory
{
	virtual ~MeshShaderFactory() {}
	virtual void Release() { delete this; }
	virtual Shader* CreateNew() { return new MeshShader(); }
};
