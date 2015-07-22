#pragma once

class Shader : public RefObject<IShader>
{
public:
	struct Property
	{
		int type;
		size_t offset;
	};
	struct Pass
	{
		ID3D11VertexShader* VS;
		ID3D11PixelShader* PS;
		ID3D11BlendState* BlendState;
		D3D11_CULL_MODE CullMode;
		ID3D11DepthStencilState* DepthStencilState;
		size_t propertyCount;
		Property* properties;
		Pass();
		virtual ~Pass();
	};
public:
	Shader();
	virtual ~Shader();
	size_t GetPassCount() const { return mPasses.size(); }
	const Pass* GetPass(size_t index) const { return mPasses[index]; }
private:
	std::vector<Pass*> mPasses;
};
