#pragma once

class ImageData
{
public:
	ImageData();
	virtual ~ImageData();
	BOOL LoadFromFile(const char* filename);
	UINT GetWidth() const { return mWidth; }
	UINT GetHegiht() const { return mHeight; }
	UINT GetBPP() const { return mBPP; }
	const BYTE* GetBits() const { return mBits; }
private:
	UINT mWidth;
	UINT mHeight;
	UINT mBPP; // 8, 24, 32
	BYTE* mBits;
};
