#include "stdafx.h"
#include "Core.h"
#include "ConstantBufferManager.h"

ConstantBufferManager gConstantBufferManager;

ConstantBuffer::ConstantBuffer()
	: mSlot(0)
	, mData(nullptr)
	, mSize(0)
	, mDirty(false)
	, mBuff(nullptr)
{

}

ConstantBuffer::~ConstantBuffer()
{
	SAFE_DELETE(mData);
	SAFE_RELEASE(mBuff);
}

bool ConstantBuffer::Init(size_t slot, size_t size)
{
	ID3D11DeviceContext* context = gCore.GetContext();
	Resize(size);
	context->VSSetConstantBuffers(slot, 1, &mBuff);
	context->PSSetConstantBuffers(slot, 1, &mBuff);
	return true;
}

bool ConstantBuffer::Commit()
{
	bool ret = false;
	ID3D11DeviceContext* context = gCore.GetContext();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CHECK(SUCCEEDED(context->Map(mBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)));
	memcpy(mappedResource.pData, mData, mSize);
	context->Unmap(mBuff, 0);
	mDirty = false;
	ret = true;
Exit0:
	return ret;
}

bool ConstantBuffer::Resize(size_t size)
{
	bool ret = false;
	D3D11_BUFFER_DESC buffDesc;
	ID3D11Device* device = gCore.GetDevice();
	CHECK(size > 0);
	CHECK(device);
	ZeroMemory(&buffDesc, sizeof(buffDesc));
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.ByteWidth = size;
	CHECK(SUCCEEDED(device->CreateBuffer(&buffDesc, nullptr, &mBuff)));
	mSize = size;
	SAFE_DELETE(mData);
	mData = new char[mSize];
	ZeroMemory(mData, mSize);
	ret = true;
Exit0:
	return ret;
}

bool ConstantBuffer::SetData(size_t offset, const void* data, size_t size)
{
	bool ret = false;
	CHECK(offset + size <= mSize);
	memcpy(mData + offset, data, size);
	mDirty = true;
	ret = true;
Exit0:
	return ret;
}

ConstantBufferManager::ConstantBufferManager()
{

}

ConstantBufferManager::~ConstantBufferManager()
{

}

void ConstantBufferManager::Release()
{
	DeleteList(mConstantBuffer);
}

void ConstantBufferManager::CreateCB(size_t slot, size_t size)
{
	if (ConstantBuffer* cb = new ConstantBuffer())
	{
		cb->Init(slot, size);
		mConstantBuffer[slot] = cb;
	}
}

bool ConstantBufferManager::Init()
{
	mConstantBuffer.resize(eCBCount, nullptr);
	CreateCB(eCBProperty, sizeof(Matrix) * 5);
	CreateCB(eCBGlobal, sizeof(CBGlobal));
	CreateCB(eCBModel, sizeof(CBModel));
	return true;
}
