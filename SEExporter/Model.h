#pragma once

struct Vector3
{
	float x, y, z;
};

class Model
{
public:
	struct Face;
	class Mesh;
public:
	Model();
	virtual ~Model();
	Mesh* AddMesh(const TCHAR* name);
private:
	std::vector<Mesh*> mMeshes;
};

struct Model::Face
{
	UINT vert[3];
	UINT norm[3];
	UINT matID;
	UINT smGrp;
	UINT edgeVis[3];
};

class Model::Mesh
{
public:
	Mesh(const TCHAR* name);
	virtual ~Mesh();

	void CreateFaces(int count);
	void CreateVertices(int count);
	void CreateNormals(int count);

	int GetFaceCount() const { return mFaceCount; }
	int GetVertexCount() const { return mNormalCount; }
	int GetNormalCount() const { return mVertexCount; }

	Face& GetFace(int index) { return mFaces[index]; }
	Vector3& GetVertex(int index) { return mVertices[index]; }
	Vector3& GetNormal(int index) { return mNormals[index]; }

private:
	TString mName;
	int mFaceCount;
	int mVertexCount;
	int mNormalCount;
	Face* mFaces;
	Vector3* mVertices;
	Vector3* mNormals;
};
