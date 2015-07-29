#pragma once

class Texture : public TRefObject<ITexture>
{
public:
	Texture();
	virtual ~Texture();
	virtual bool LoadFromFile(const char* filename);
	bool Create(UINT w, UINT h, const void* bits);
	virtual int GetWidth() const { return mWidth; }
	virtual int GetHeight() const { return mHeight; }
	ID3D11ShaderResourceView* GetSRV() const { return mSRV; }
	ID3D11SamplerState* GetSamplerState() const { return mSampler; }
private:
	int mWidth;
	int mHeight;
	ID3D11Texture2D* mPtr;
	ID3D11ShaderResourceView* mSRV;
	ID3D11SamplerState* mSampler;
};
