#pragma once

class Mesh : public TRefObject<IMesh>
{
public:
	Mesh();
	virtual ~Mesh();
	virtual bool LoadFromFile(const char* filename);
	bool Create(const void* vertices, size_t vertexSize, size_t vertexCount, const USHORT* indices, size_t indexCount);
	size_t GetBoneCount() { return mBoneCount; }
	void Draw();
	bool Setup();
private:
	size_t mVertexSize;
	size_t mIndexCount;
	size_t mBoneCount;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
};
