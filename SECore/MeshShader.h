#pragma once

class MeshShader : public Shader
{
public:
	struct Buffer
	{
		Color color;
		float cutoff;
	};
public:
	virtual ~MeshShader();
	virtual bool Init();
	virtual void Release() { delete this; }
	virtual bool SetPass(size_t pass) const;
	virtual size_t GetPassCount() const { return 1; }
	virtual bool SetValue(const char* name, const void* data);
	virtual bool SetTexture(const char* name, ITexture* texture);
	virtual PropertyType GetPropertyType(const char* name) const;
public:
	MeshShader();
private:
	Buffer mBuffer;
	Texture* mMainTexture;
	ID3D11PixelShader* mPS;
};

struct MeshShaderFactory : ShaderFactory
{
	virtual ~MeshShaderFactory() {}
	virtual void Release() { delete this; }
	virtual Shader* CreateNew() { return new MeshShader(); }
};
