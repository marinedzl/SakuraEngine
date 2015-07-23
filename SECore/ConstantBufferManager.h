#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer();
	virtual ~ConstantBuffer();
	bool Init(size_t slot, size_t size);
	size_t GetSize() const { return mSize; }
	const char* GetData() const { return mData; }
	char* GetData() { return mData; }
	bool SetData(size_t offset, const void* data, size_t size);
	bool Resize(size_t size);
	bool IsDirty() const { return mDirty; }
	bool Commit();
private:
	size_t mSlot;
	char* mData;
	size_t mSize;
	bool mDirty;
	ID3D11Buffer* mBuff;
};

class ConstantBufferManager
{
public:
	ConstantBufferManager();
	virtual ~ConstantBufferManager();
	void Release();
	bool Init();
	ConstantBuffer* GetBuffer(size_t slot) { return mConstantBuffer.at(slot); }
	template<typename T> T* GetBuffer() { return (T*)GetBuffer(T::slot)->GetData(); }
	template<typename T> void Commit() { GetBuffer(T::slot)->Commit(); }
private:
	void CreateCB(size_t slot, size_t size);
private:
	std::vector<ConstantBuffer*> mConstantBuffer;
};

extern ConstantBufferManager gConstantBufferManager;
