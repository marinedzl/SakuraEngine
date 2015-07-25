#include "stdafx.h"
#include "Model.h"

Model::Mesh::Mesh(const TCHAR* name)
	: mName(name)
	, mFaceCount(0)
	, mVertexCount(0)
	, mNormalCount(0)
	, mFaces(nullptr)
	, mVertices(nullptr)
	, mNormals(nullptr)
{

}

Model::Mesh::~Mesh()
{
	SAFE_DELETE(mFaces);
	SAFE_DELETE(mVertices);
	SAFE_DELETE(mNormals);
}

void Model::Mesh::CreateFaces(int count)
{
	SAFE_DELETE(mFaces);
	mFaceCount = count;
	mFaces = new Face[count];
}

void Model::Mesh::CreateVertices(int count)
{
	SAFE_DELETE(mVertices);
	mVertexCount = count;
	mVertices = new Vector3[count];
}

void Model::Mesh::CreateNormals(int count)
{
	SAFE_DELETE(mNormals);
	mNormalCount = count;
	mNormals = new Vector3[count];
}

Model::Model()
{
}

Model::~Model()
{
	DeleteList(mMeshes);
}

Model::Mesh * Model::AddMesh(const TCHAR * name)
{
	Mesh* mesh = new Mesh(name);
	mMeshes.push_back(mesh);
	return mesh;
}
