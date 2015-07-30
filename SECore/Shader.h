#pragma once

class Shader : public TRefObject<IShader>
{
	friend class ShaderCompiler;
public:
	struct Property
	{
		enum Type
		{
			Float,
			Vector3,
			Vector4,
			Color,
			Texture2D,
		};
		Type type;
		size_t offset;
	};
	struct Pass
	{
	public:
		Pass();
		virtual ~Pass();
		Property* AddProperty(const char* name);
		const Property* GetProperty(const char* name) const;
	public:
		ID3D11VertexShader* VS;
		ID3D11PixelShader* PS;
		ID3D11BlendState* BlendState;
		D3D11_CULL_MODE CullMode;
		ID3D11DepthStencilState* DepthStencilState;
		size_t buffSize;
		size_t textureCount;
	private:
		typedef std::map<std::string, Property*> Properties;
	private:
		Properties mProperties;
	};
public:
	Shader();
	virtual ~Shader();
	virtual bool LoadFromFile(const char* filename);
	size_t GetPassCount() const { return mPasses.size(); }
	const Pass* GetPass(size_t index) const { return mPasses[index]; }
	const Property* GetProperty(const char* name) const;
private:
	std::vector<Pass*> mPasses;
};
