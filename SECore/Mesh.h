#pragma once

class Mesh : public TRefObject<SECore::Mesh>
{
public:
	Mesh();
	virtual ~Mesh();
	virtual bool LoadFromFile(const char* filename);
	bool Create(const void* vertices, size_t vertexSize, size_t vertexCount, const USHORT* indices, size_t indexCount);
	void Draw();
	bool Setup();
	const Bound& GetBound() const { return mBound; }
private:
	size_t mVertexSize;
	size_t mIndexCount;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	Bound mBound;
};
