#include "stdafx.h"
#include <FreeImage.h>
#include "ImageData.h"

ImageData::ImageData()
	: mWidth(0)
	, mHeight(0)
	, mBPP(0)
	, mBits(nullptr)
{
}

ImageData::~ImageData()
{
	SAFE_DELETE(mBits);
}

BOOL ImageData::LoadFromFile(const char* filename)
{
	BOOL ret = FALSE;
	FIBITMAP* dib = nullptr;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);

	CHECK(fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif));

	dib = FreeImage_Load(fif, filename);
	CHECK(dib);

	mBPP = FreeImage_GetBPP(dib);
	mWidth = FreeImage_GetWidth(dib);
	mHeight = FreeImage_GetHeight(dib);

	UINT pitch = FreeImage_GetPitch(dib);
	BYTE* bits = FreeImage_GetBits(dib);
	CHECK(bits);

	switch (mBPP)
	{
	case 8:
	{
		RGBQUAD* palette = FreeImage_GetPalette(dib);
		CHECK(palette);

		mBits = new BYTE[mWidth * mHeight * 4];
		CHECK(mBits);

		for (UINT i = 0; i < mHeight; ++i) {
			for (UINT j = 0; j < mWidth; ++j) {
				UINT k = bits[i * pitch + j];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 0] = palette[k].rgbRed;
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 1] = palette[k].rgbGreen;
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 2] = palette[k].rgbBlue;
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 3] = 255;
			}
		}
	}
	break;
	case 24:
	{
		mBits = new BYTE[mWidth * mHeight * 4];
		CHECK(mBits);

		for (UINT i = 0; i < mHeight; ++i) {
			for (UINT j = 0; j < mWidth; ++j) {
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 0] = bits[i * pitch + j * 3 + 2];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 1] = bits[i * pitch + j * 3 + 1];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 2] = bits[i * pitch + j * 3 + 0];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 3] = 255;
			}
		}
	}
	break;
	case 32:
	{
		mBits = new BYTE[mWidth * mHeight * 4];
		CHECK(mBits);

		for (UINT i = 0; i < mHeight; ++i) {
			for (UINT j = 0; j < mWidth; ++j) {
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 0] = bits[i * pitch + j * 4 + 2];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 1] = bits[i * pitch + j * 4 + 1];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 2] = bits[i * pitch + j * 4 + 0];
				mBits[((mHeight - 1 - i) * mWidth + j) * 4 + 3] = bits[i * pitch + j * 4 + 3];
			}
		}
	}
	break;
	default:
		break;
	}

	ret = TRUE;
Exit0:
	if (dib)
		FreeImage_Unload(dib);
	return ret;
}
