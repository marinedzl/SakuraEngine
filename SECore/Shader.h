#pragma once
#include "AnyValue.h"

class Shader : public TRefObject<IShader>
{
	friend class ShaderCompiler;
public:
	class Property
	{
	public:
		Property() : offset(0) {}
		~Property() { }
		size_t GetOffset() const { return offset; }
		void SetOffset(size_t offset) { this->offset = offset; }
		void SetValue(const AnyValue& value) { this->value = value; }
		const AnyValue& GetValue() const { return value; }
	private:
		AnyValue value;
		size_t offset;
	};
	struct Pass
	{
	public:
		Pass();
		virtual ~Pass();
		Property* AddProperty(const char* name);
		const Property* GetProperty(const char* name) const;
		bool InitMaterial(Material* material) const;
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
