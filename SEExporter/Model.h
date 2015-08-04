#pragma once
#include "Utility.h"

namespace MaxPlugin
{
	namespace VertexChannel
	{
		enum Type
		{
			Position = 0x1,
			Normal = 0x2,
			Tangent = 0x4,
			TexCoords0 = 0x8,
			TexCoords1 = 0x10,
			TexCoords2 = 0x20,
			BoneWeights = 0x40,
		};
	};

	struct BoneWeight
	{
		union
		{
			byte id[4];
			uint uid;
		};
		float weight[3];
	};

	struct Vertex
	{
		Vec3 pos;
		Vec3 norm;
		Vec2 uv0;
		Vec2 uv1;
		Vec2 uv2;
		Vec3 tan;
		BoneWeight bw;
	};

	struct Mesh
	{
		TString name;
		TString DiffTex;
		TString BumpTex;
		TString SpecTex;
		UINT vertexChannels;
		std::vector<Vertex> vertices;
		std::vector<USHORT> indices;
	};

	class MeshExtracter;

	class Model
	{
	public:
		Model();
		virtual ~Model();
		void Clear();
		bool Extract(IGameScene* gScene);
		void WriteMesh(const Mesh* mesh, const TCHAR* path);
		void WriteFile(const TCHAR* filename);
		void AddMesh(Mesh* mesh) { mMeshes.push_back(mesh); }
	private:
		void ExtractNode(IGameNode* gNode);
	private:
		MeshExtracter* mMeshExtracter;
		std::vector<Mesh*> mMeshes;
	};
}
