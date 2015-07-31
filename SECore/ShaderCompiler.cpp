#include "stdafx.h"
#include "Shader.h"
#include "RenderStateManager.h"
#include "Core.h"
#include "ShaderCompiler.h"

/*

ZWrite (On | Off)
Cull (Back | Front | Off)
BlendOp (Add | Sub | RevSub | Min | Max)
Blend (One | Zero | SrcColor | SrcAlpha | DstColor | DstAlpha | OneMinusSrcColor | OneMinusSrcAlpha | OneMinusDstColor | OneMinusDstAlpha)
ZTest (Off | Less | Greater | LEqual | GEqual | Equal | NotEqual | Always)
AlphaTest (Off | Less | Greater | LEqual | GEqual | Equal | NotEqual | Always) CutoffValue

*/

namespace
{
	bool WriteFile(const void* data, size_t size, const char* filename)
	{
		bool ret = false;
		FILE* file = nullptr;

		CHECK(data && size > 0 && filename);

		fopen_s(&file, filename, "wb");
		CHECK(file);

		fwrite(data, 1, size, file);

		ret = true;
	Exit0:
		if (file)
			fclose(file);
		return ret;
	}
}

ShaderCompiler gShaderCompiler;

ShaderCompiler::ShaderCompiler()
{
}

ShaderCompiler::~ShaderCompiler()
{
}

void ShaderCompiler::Release()
{

}

bool ShaderCompiler::Init()
{
	mBlendOpMap.insert(std::make_pair("Add", D3D11_BLEND_OP_ADD));
	mBlendOpMap.insert(std::make_pair("Sub", D3D11_BLEND_OP_SUBTRACT));
	mBlendOpMap.insert(std::make_pair("RevSub", D3D11_BLEND_OP_REV_SUBTRACT));
	mBlendOpMap.insert(std::make_pair("Min", D3D11_BLEND_OP_MIN));
	mBlendOpMap.insert(std::make_pair("Max", D3D11_BLEND_OP_MAX));

	mBlendMap.insert(std::make_pair("One", D3D11_BLEND_ONE));
	mBlendMap.insert(std::make_pair("Zero", D3D11_BLEND_ZERO));
	mBlendMap.insert(std::make_pair("SrcColor", D3D11_BLEND_SRC_COLOR));
	mBlendMap.insert(std::make_pair("SrcAlpha", D3D11_BLEND_SRC_ALPHA));
	mBlendMap.insert(std::make_pair("DstColor", D3D11_BLEND_DEST_COLOR));
	mBlendMap.insert(std::make_pair("DstAlpha", D3D11_BLEND_DEST_ALPHA));
	mBlendMap.insert(std::make_pair("OneMinusSrcColor", D3D11_BLEND_INV_SRC_COLOR));
	mBlendMap.insert(std::make_pair("OneMinusSrcAlpha", D3D11_BLEND_INV_SRC_ALPHA));
	mBlendMap.insert(std::make_pair("OneMinusDstColor", D3D11_BLEND_INV_DEST_COLOR));
	mBlendMap.insert(std::make_pair("OneMinusDstAlpha", D3D11_BLEND_INV_DEST_ALPHA));

	mZTestMap.insert(std::make_pair("Off", D3D11_COMPARISON_NEVER));
	mZTestMap.insert(std::make_pair("Less", D3D11_COMPARISON_LESS));
	mZTestMap.insert(std::make_pair("Greater", D3D11_COMPARISON_GREATER));
	mZTestMap.insert(std::make_pair("LEqual", D3D11_COMPARISON_LESS_EQUAL));
	mZTestMap.insert(std::make_pair("GEqual", D3D11_COMPARISON_GREATER_EQUAL));
	mZTestMap.insert(std::make_pair("Equal", D3D11_COMPARISON_EQUAL));
	mZTestMap.insert(std::make_pair("NotEqual", D3D11_COMPARISON_NOT_EQUAL));
	mZTestMap.insert(std::make_pair("Always", D3D11_COMPARISON_ALWAYS));

	mAlphaTestMap.insert(std::make_pair("Less", AlphaTestLess));
	mAlphaTestMap.insert(std::make_pair("Greater", AlphaTestGreater));
	mAlphaTestMap.insert(std::make_pair("LEqual", AlphaTestLEqual));
	mAlphaTestMap.insert(std::make_pair("GEqual", AlphaTestGEqual));
	mAlphaTestMap.insert(std::make_pair("Equal", AlphaTestEqual));
	mAlphaTestMap.insert(std::make_pair("NotEqual", AlphaTestNotEqual));
	mAlphaTestMap.insert(std::make_pair("Always", AlphaTestAlways));

	mPropertyMap.insert(std::make_pair("Float", eFloat));
	mPropertyMap.insert(std::make_pair("Vector3", eVector3));
	mPropertyMap.insert(std::make_pair("Vector4", eVector4));
	mPropertyMap.insert(std::make_pair("Color", eColor));
	mPropertyMap.insert(std::make_pair("Texture2D", eTexture2D));

	return true;
}

bool ShaderCompiler::StringToBlendOp(const std::string& str, D3D11_BLEND_OP& dst)
{
	BlendOpMap::iterator iter = mBlendOpMap.find(str);
	if (iter == mBlendOpMap.end())
	{
		return false;
	}
	else
	{
		dst = iter->second;
		return true;
	}
}

bool ShaderCompiler::String2Blend(const std::string& str, D3D11_BLEND& dst)
{
	BlendMap::iterator iter = mBlendMap.find(str);
	if (iter == mBlendMap.end())
	{
		return false;
	}
	else
	{
		dst = iter->second;
		return true;
	}
}

bool ShaderCompiler::StringToCullMode(const std::string& str, D3D11_CULL_MODE& dst)
{
	if (str == "Back")
		dst = D3D11_CULL_BACK;
	else if (str == "Front")
		dst = D3D11_CULL_FRONT;
	else if (str == "Off")
		dst = D3D11_CULL_NONE;
	else
		return false;
	return true;
}

bool ShaderCompiler::StringToZWrite(const std::string& str, BOOL& dst)
{
	if (str == "On")
		dst = TRUE;
	else if (str == "Off")
		dst = FALSE;
	else
		return false;
	return true;
}

bool ShaderCompiler::StringToZTest(const std::string& str, D3D11_COMPARISON_FUNC& dst)
{
	ZTestMap::iterator iter = mZTestMap.find(str);
	if (iter == mZTestMap.end())
	{
		return false;
	}
	else
	{
		dst = iter->second;
		return true;
	}
}

bool ShaderCompiler::String2AlphaTest(const std::string& str, AlphaTestMode& dst)
{
	AlphaTestMap::iterator iter = mAlphaTestMap.find(str);
	if (iter == mAlphaTestMap.end())
	{
		return false;
	}
	else
	{
		dst = iter->second;
		return true;
	}
}

bool ShaderCompiler::String2Proprety(const std::string& str, ValueType& dst)
{
	PropertyMap::iterator iter = mPropertyMap.find(str);
	if (iter == mPropertyMap.end())
	{
		return false;
	}
	else
	{
		dst = iter->second;
		return true;
	}
}

bool ShaderCompiler::ParsePass(Shader* shader, const Json::Value& root, const std::string& path)
{
	bool ret = false;
	ID3D11Device* device = gCore.GetDevice();

	Shader::Pass* pass = new Shader::Pass();
	shader->mPasses.push_back(pass);

	D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;

	BOOL ZWriteEnable = TRUE;
	D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	BOOL AlphaTestEnable = FALSE;
	AlphaTestMode alphaTestMode = AlphaTestLEqual;
	float CutoffValue = 0.5f;

	BOOL BlendEnable = FALSE;
	D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD;
	D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA;
	D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	std::vector<std::string> memberNames = root.getMemberNames();
	size_t memberCount = memberNames.size();
	for (size_t i = 0; i < memberCount; ++i)
	{
		const Json::Value& child = root[memberNames[i]];

		if (memberNames[i] == "BufferSize")
		{
			pass->buffSize = child.asInt();
		}
		else if (memberNames[i] == "TextureCount")
		{
			pass->textureCount = child.asInt();
		}
		else if (memberNames[i] == "PS")
		{
			std::string filename = path + "\\" + child.asString();
			buffer file;
			ID3D11PixelShader* s = nullptr;
			if (LoadBinaryFile(file, filename.c_str()))
				device->CreatePixelShader(file.ptr(), file.size(), nullptr, &s);
			pass->PS = s;
		}
		else if (memberNames[i] == "VS")
		{
			std::string filename = path + "\\" + child.asString();
			buffer file;
			ID3D11VertexShader* s = nullptr;
			if (LoadBinaryFile(file, filename.c_str()))
				device->CreateVertexShader(file.ptr(), file.size(), nullptr, &s);
			pass->VS = s;
		}
		else if (memberNames[i] == "Properties")
		{
			CHECK(child.isArray());
			size_t propCount = child.size();
			for (size_t pi = 0; pi < propCount; pi++)
			{
				const Json::Value& props = child[pi];
				CHECK(props.isArray());
				CHECK(props.size() >= 3);
				std::string type = props[size_t(0)].asString();
				std::string name = props[1].asString();
				Shader::Property* prop = pass->AddProperty(name.c_str());
				ValueType etype = eNull;
				CHECK(String2Proprety(type, etype));
				prop->SetOffset(props[2].asInt());
				if (props.size() > 3)
				{
					AnyValue value;
					CHECK(String2Value(props[3], etype, value));
					prop->SetValue(value);
				}
				else
				{
					prop->SetValue(AnyValue(etype));
				}
			}
		}
		else
		{
			CHECK(false);
		}
	}

	// CreateRasterizerState
	{
		pass->CullMode = CullMode;
	}

	// CreateDepthStencilState
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = DepthFunc != D3D11_COMPARISON_NEVER;
		desc.DepthWriteMask = ZWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = DepthFunc;
		desc.StencilEnable = FALSE;
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		pass->DepthStencilState = gRenderStateManager.GetRenderState<ID3D11DepthStencilState>(desc);
	}

	// CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = FALSE;
		desc.IndependentBlendEnable = FALSE; // 不针对多个RenderTarget使用不同的混合状态
		desc.RenderTarget[0].BlendEnable = BlendEnable;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 即使是disable也要填这个参数

		if (BlendEnable)
		{
			desc.RenderTarget[0].SrcBlend = SrcBlend;
			desc.RenderTarget[0].DestBlend = DestBlend;
			desc.RenderTarget[0].BlendOp = BlendOp;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		}

		pass->BlendState = gRenderStateManager.GetRenderState<ID3D11BlendState>(desc);
	}

	ret = true;
Exit0:
	return ret;
}

bool ShaderCompiler::LoadShader(Shader* shader, const char* filename)
{
	bool ret = false;
	Json::Reader reader;
	Json::Value root;
	SplitPath sp;
	buffer file;
	std::string path;
	CHECK(shader && filename);
	CHECK(LoadTextFile(file, filename) && file.size() > 0);

	sp.Split(filename);
	path = sp.GetPath();

	if (!reader.parse(file.ptr(), root))
	{
		log(reader.getFormatedErrorMessages().c_str());
		CHECK(false);
	}
	CHECK(root.isArray());

	int count = root.size();
	for (int i = 0; i < count; ++i)
	{
		ParsePass(shader, root[i], path);
	}

	ret = true;
Exit0:
	return ret;
}
