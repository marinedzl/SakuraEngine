#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderStateManager.h"
#include "ConstantBufferManager.h"
#include "Core.h"
#include "Material.h"

class Material::Pass
{
public:
	Pass() {}
	virtual ~Pass() { ReleaseArray(mTextures); }
	bool Init(size_t buffSize, size_t textureCount)
	{
		if (buffSize > 0)
		{
			mBuffer.resize(buffSize);
		}
		if (textureCount > 0)
		{
			mTextures.resize(textureCount);
			for (size_t i = 0; i < textureCount; i++)
				mTextures[i] = nullptr;
		}
		return true;
	}
	size_t GetBufferSize() const { return mBuffer.size(); }
	const char* GetBuffer() const { return mBuffer.ptr(); }
	void SetData(const char* data, size_t offset, size_t size)
	{
		memcpy(mBuffer.ptr() + offset, data, size);
	}
	size_t GetTextureCount() const { return mTextures.size(); }
	Texture* GetTexture(size_t index) const { return mTextures[index]; }
	void SetTexture(Texture* texture, size_t index)
	{
		CHECK(index < mTextures.size());
		SAFE_RELEASE(mTextures[index]);
		mTextures[index] = texture;
		texture->AddRef();
	Exit0:
		;
	}
private:
	array<Texture*> mTextures;
	buffer mBuffer;
};

Material::Material()
{
}

Material::~Material()
{
	Clear();
}

void Material::Clear()
{
	SAFE_RELEASE(mShader);
	DeleteArray(mPasses);
}

void Material::SetShader(IShader* shader)
{
	Clear();
	if (Shader* s = dynamic_cast<Shader*>(shader))
	{
		mShader = s;
		mShader->AddRef();
	}
	Reload();
}

void Material::Reload()
{
	if (!mShader)
		return;

	size_t count = mShader->GetPassCount();
	mPasses.resize(count);

	for (size_t i = 0; i < count; i++)
	{
		const Shader::Pass* spass = mShader->GetPass(i);
		Pass* pass = new Pass();
		pass->Init(spass->buffSize, spass->textureCount);
		mPasses[i] = pass;
	}
}

void Material::Setup(size_t passIndex)
{
	ID3D11DeviceContext* context = gCore.GetContext();
	CHECK(context);

	CHECK(passIndex < mShader->GetPassCount());
	CHECK(passIndex < GetPassCount());

	const Shader::Pass* spass = mShader->GetPass(passIndex);
	Pass* pass = mPasses[passIndex];

	CHECK(pass && spass);

	// commit buffer
	ConstantBuffer* cb = gConstantBufferManager.GetBuffer(eCBProperty);
	if (cb)
	{
		ConstantBuffer* cb = gConstantBufferManager.GetBuffer(eCBProperty);
		cb->SetData(0, pass->GetBuffer(), pass->GetBufferSize());
		cb->Commit();
	}

	// set texture
	size_t textureCount = pass->GetTextureCount();
	for (size_t i = 0; i < textureCount; i++)
	{
		if (Texture* texture = pass->GetTexture(i))
		{
			UINT slot = texture_slot_reserve + i;
			ID3D11ShaderResourceView* srv = texture->GetSRV();
			ID3D11SamplerState* sampler = texture->GetSamplerState();
			context->PSSetShaderResources(slot, 1, &srv);
			context->PSSetSamplers(slot, 1, &sampler);
		}
	}

	float factor[4] = { 0 };
	if (spass->VS)
		context->VSSetShader(spass->VS, nullptr, 0);
	if (spass->PS)
		context->PSSetShader(spass->PS, nullptr, 0);
	context->OMSetBlendState(spass->BlendState, factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(spass->DepthStencilState, 1);
	context->RSSetState(gRenderStateManager.GetRasterizerState(spass->CullMode, D3D11_FILL_SOLID));

Exit0:
	;
}

void Material::SetFloat(const char* name, float value)
{
	SetValue(name, (const char*)&value, Shader::Property::Float, sizeof(float));
}

void Material::SetColor(const char* name, const Color& value)
{
	SetValue(name, (const char*)&value, Shader::Property::Color, sizeof(Color));
}

void Material::SetTexture(const char* name, ITexture* value)
{
	CHECK(mShader);

	Texture* texture = dynamic_cast<Texture*>(value);
	CHECK(texture);

	size_t count = mShader->GetPassCount();
	CHECK(count == mPasses.size());
	for (size_t i = 0; i < count; i++)
	{
		const Shader::Pass* spass = mShader->GetPass(i);
		if (const Shader::Property* sprop = spass->GetProperty(name))
		{
			CHECK(sprop->type == Shader::Property::Texture2D);
			Pass* pass = mPasses[i];
			CHECK(pass);
			CHECK(sprop->offset < pass->GetTextureCount());
			pass->SetTexture(texture, sprop->offset);
		}
	}
Exit0:
	;
}

void Material::SetValue(const char* name, const char* data, int type, size_t size)
{
	CHECK(mShader);

	size_t count = mShader->GetPassCount();
	CHECK(count == mPasses.size());
	for (size_t i = 0; i < count; i++)
	{
		const Shader::Pass* spass = mShader->GetPass(i);
		if (const Shader::Property* sprop = spass->GetProperty(name))
		{
			CHECK(sprop->type == type);
			Pass* pass = mPasses[i];
			CHECK(pass);
			CHECK(pass->GetBufferSize() >= sprop->offset + size);
			pass->SetData(data, sprop->offset, size);
		}
	}
Exit0:
	;
}
