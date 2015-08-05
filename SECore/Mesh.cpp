#include "stdafx.h"
#include "SECore\FileFormat.h"
#include "Core.h"
#include "Mesh.h"

struct BoneWeights
{
	byte bones[4];
	float weights[3];
};

struct Vertex
{
	Vector3 pos;
	Vector3 norm;
	Vector3 tan;
	Vector2 uv0; // Texture
	Vector2 uv1; // LightMap
	Vector2 uv2; // ShadowMap
	BoneWeights boneWeights;
};

Mesh::Mesh()
	: mVertexSize(0)
	, mIndexCount(0)
	, mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
{
}

Mesh::~Mesh()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
}

bool Mesh::Create(const void* vertices, size_t vertexSize, size_t vertexCount, const USHORT* indices, size_t indexCount)
{
	bool ret = false;
	ID3D11Device* device = gCore.GetDevice();

	CHECK(device && vertices && indices);

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA dataDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	ZeroMemory(&dataDesc, sizeof(dataDesc));

	mVertexSize = vertexSize;
	mIndexCount = indexCount;

	bufferDesc.ByteWidth = vertexSize * vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dataDesc.pSysMem = vertices;
	CHECK(SUCCEEDED(device->CreateBuffer(&bufferDesc, &dataDesc, &mVertexBuffer)));

	bufferDesc.ByteWidth = sizeof(USHORT) * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dataDesc.pSysMem = indices;
	CHECK(SUCCEEDED(device->CreateBuffer(&bufferDesc, &dataDesc, &mIndexBuffer)));

	ret = true;
Exit0:
	return ret;
}

#define LoadData(p, t)\
{\
			for (size_t _i = 0; _i < head->vertexCount; ++_i)\
				vertices[_i].##p = ((t*)data)[_i];\
			}

bool Mesh::LoadFromFile(const char* filename)
{
	bool ret = false;
	buffer file;

	CHECK(filename);
	CHECK(LoadBinaryFile(file, filename));

	const char* data = file.ptr();

	MeshFile::Head* head = (MeshFile::Head*)data;
	data += sizeof(MeshFile::Head);

	MeshFile::Block* blocks = (MeshFile::Block*)data;
	data += sizeof(MeshFile::Block) * head->blockCount;

	Vertex* vertices = new Vertex[head->vertexCount];
	USHORT* indices = nullptr;

	for (size_t i = 0; i < head->blockCount; i++)
	{
		MeshFile::Block::Type type = blocks[i].type;

		switch (type)
		{
		case MeshFile::Block::Face:
			indices = (USHORT*)data;
			break;
		case MeshFile::Block::Position:
			LoadData(pos, Vector3);
			break;
		case MeshFile::Block::Normal:
			LoadData(norm, Vector3);
			break;
		case MeshFile::Block::Tangent:
			LoadData(tan, Vector3);
			break;
		case MeshFile::Block::TexCoords0:
			LoadData(uv0, Vector2);
			break;
		case MeshFile::Block::TexCoords1:
			LoadData(uv1, Vector2);
			break;
		case MeshFile::Block::TexCoords2:
			LoadData(uv2, Vector2);
			break;
		case MeshFile::Block::BoneWeights:
			LoadData(boneWeights, BoneWeights);
			break;
		default:
			break;
		}
		data += blocks[i].size;
	}

	ret = Create(vertices, sizeof(Vertex), head->vertexCount, indices, head->indexCount);
Exit0:
	SAFE_DELETE(vertices);
	return ret;
}

bool Mesh::Setup()
{
	ID3D11DeviceContext* context = gCore.GetContext();
	UINT stride = mVertexSize;
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	return true;
}

void Mesh::Draw()
{
	ID3D11DeviceContext* context = gCore.GetContext();
	context->DrawIndexed(mIndexCount, 0, 0);
}
