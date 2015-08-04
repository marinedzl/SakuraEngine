#pragma once
#include "Model.h"

namespace MaxPlugin
{
	class MeshExtracter
	{
		friend class Model;
	public:
		MeshExtracter();
		~MeshExtracter();
		void Clear();
		void Extract(IGameNode* gNode);
		void GetData(Model* model);
	private:
		struct Face
		{
			uint vert[3];
			uint uv[3];
			uint norm[3];
		};

		struct SubMesh
		{
			TString name;
			TString DiffTex;
			TString BumpTex;
			TString SpecTex;
			UINT vertexChannels;
			std::vector<Face> faces;
		};

		struct MaxMesh
		{
			std::vector<Vec3> P; // position
			std::vector<Vec3> N; // normal
			std::vector<Vec2> UV[2];
			std::vector<BoneWeight> BW; // blend weight and indices
			void Clear()
			{
				P.clear();
				N.clear();
				UV[0].clear();
				UV[1].clear();
				BW.clear();
			}
		};
	private:
		void ExtractGeom(IGameMesh* gMesh);
		void ExtractSubMesh(IGameNode* gNode, IGameMesh* gMesh);
		void ExtractSkinInfo(IGameObject * obj);
		uint GetBoneID(const TCHAR* name);
		void ExtractFace(Face& face, const FaceEx* src);
		void ExtractMat(SubMesh * subMesh, IGameMaterial * gMaterial);
		void GetSubMeshData(Model* model, const SubMesh* subMesh);
	private:
		std::vector<SubMesh*> mSubMeshes;
		MaxMesh mMaxMesh;
	};
}
