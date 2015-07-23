#pragma once

class Shader : public RefObject<IShader>
{
public:
	struct Property
	{
		enum Type
		{
			Float,
			Vector3,
			Vector4,
			Color,
			Texture,
		};
		int type;
		size_t offset;
		char name[32];
	};
	struct Pass
	{
	public:
		Pass();
		virtual ~Pass();
		const Property* GetProperty(const char* name) const;
	public:
		ID3D11VertexShader* VS;
		ID3D11PixelShader* PS;
		ID3D11BlendState* BlendState;
		D3D11_CULL_MODE CullMode;
		ID3D11DepthStencilState* DepthStencilState;
	private:
		typedef std::map<std::string, Property> Properties;
	private:
		Properties mProperties;
	};
public:
	Shader();
	virtual ~Shader();
	size_t GetPassCount() const { return mPasses.size(); }
	const Pass* GetPass(size_t index) const { return mPasses[index]; }
private:
	std::vector<Pass*> mPasses;
};
