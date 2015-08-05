#include "stdafx.h"
#include "Skeleton.h"
#include "MeshExtracter.h"

namespace MaxPlugin
{
	IGameMaterial* GetMaterialById(IGameMaterial *mat, int id)
	{
		for (int i = 0; i < mat->GetSubMaterialCount(); i++)
			if (mat->GetMaterialID(i) == id)
				return mat->GetSubMaterial(i);

		return NULL;
	}

	MeshExtracter::MeshExtracter()
	{
	}

	MeshExtracter::~MeshExtracter()
	{
		Clear();
	}

	void MeshExtracter::Clear()
	{
		DeleteList(mSubMeshes);
		mMaxMesh.Clear();
	}

	void MeshExtracter::Extract(IGameNode* gNode)
	{
		Clear();

		IGameObject* obj = gNode->GetIGameObject();
		IGameMesh* gMesh = (IGameMesh*)obj;

		gMesh->SetCreateOptimizedNormalList();
		obj->InitializeData();

		ExtractGeom(gMesh);
		ExtractSkinInfo(obj);
		ExtractSubMesh(gNode, gMesh);

		gNode->ReleaseIGameObject();
	}

	void MeshExtracter::ExtractGeom(IGameMesh* gMesh)
	{
		// position
		int vertCount = gMesh->GetNumberOfVerts();
		mMaxMesh.P.resize(vertCount);
		for (int i = 0; i < vertCount; ++i)
			Utility::ToVec3(mMaxMesh.P[i], gMesh->GetVertex(i));

		// normal
		int normCount = gMesh->GetNumberOfNormals();
		mMaxMesh.N.resize(normCount);
		for (int i = 0; i < normCount; ++i)
		{
			Utility::ToVec3(mMaxMesh.N[i], gMesh->GetNormal(i));
		}

		Tab<int> texMaps = gMesh->GetActiveMapChannelNum();

		// texcoord
		for (int i = 0; i < texMaps.Count(); ++i)
		{
			int chan = texMaps[i];
			int uvCount = gMesh->GetNumberOfMapVerts(chan);
			mMaxMesh.UV[i].resize(uvCount);
			for (int j = 0; j < uvCount; ++j)
			{
				Point3 tv = gMesh->GetMapVertex(chan, j);
				mMaxMesh.UV[i][j].x = tv.x;
				mMaxMesh.UV[i][j].y = 1 - tv.y;
			}
		}
	}

	void MeshExtracter::ExtractSubMesh(IGameNode* gNode, IGameMesh* gMesh)
	{
		IGameMaterial* mat = gNode->GetNodeMaterial();

		DWORD vertexChannels = VertexChannel::Position | VertexChannel::Normal;

		if (gMesh->GetNumberOfMapVerts(1) > 0)
			vertexChannels |= VertexChannel::TexCoords0;

		if (gMesh->GetNumberOfMapVerts(2) > 0)
			vertexChannels |= VertexChannel::TexCoords1;

		if (gMesh->GetNumberOfMapVerts(3) > 0)
			vertexChannels |= VertexChannel::TexCoords2;

		if (!mMaxMesh.BW.empty())
			vertexChannels |= VertexChannel::BoneWeights;

		if (mat == NULL || !mat->IsMultiType())
		{
			SubMesh* mesh = new SubMesh();
			mesh->name = gNode->GetName();
			mesh->vertexChannels = vertexChannels;
			mSubMeshes.push_back(mesh);

			if (mat != NULL)
			{
				ExtractMat(mesh, mat);
			}
			else
				log(_T("no material found for %s\n"), gNode->GetName());

			int faceCount = gMesh->GetNumberOfFaces();
			mesh->faces.resize(faceCount * 3);
			for (int i = 0; i < faceCount; ++i)
				ExtractFace(mesh->faces[i], gMesh->GetFace(i));
		}
		else
		{
			log(_T("node %s has multi type material %s\n"), gNode->GetName(), mat->GetMaterialName());

			Tab<int> matIds = gMesh->GetActiveMatIDs();

			for (int i = 0; i < matIds.Count(); ++i)
			{
				void* mtlid = 0;
				TSTR meshName;

				IGameMaterial* subMat = GetMaterialById(mat, matIds[i]);
				if (subMat != NULL)
				{
					mtlid = subMat;
					meshName.printf(_T("%s_%s"), gNode->GetName(), subMat->GetMaterialName());
				}
				else
				{
					meshName.printf(_T("%s_%s"), gNode->GetName(), i);
				}

				SubMesh* mesh = new SubMesh();
				mesh->name = meshName;
				mesh->vertexChannels = vertexChannels;
				mSubMeshes.push_back(mesh);
				ExtractMat(mesh, subMat);

				Tab<FaceEx*> gFaces = gMesh->GetFacesFromMatID(matIds[i]);

				int faceCount = gFaces.Count();
				mesh->faces.resize(faceCount * 3);
				for (int j = 0; j < faceCount; ++j)
					ExtractFace(mesh->faces[j], gFaces[j]);
			}
		}
	}

	void MeshExtracter::ExtractSkinInfo(IGameObject * obj)
	{
		IGameSkin * Skin = obj->GetIGameSkin();

		if (!Skin)
			return;

		int skinnedVertCount = Skin->GetNumOfSkinnedVerts();

		CHECK(skinnedVertCount == mMaxMesh.P.size());

		mMaxMesh.BW.resize(skinnedVertCount);

		for (int i = 0; i < skinnedVertCount; i++)
		{
			int type = Skin->GetVertexType(i);

			BoneWeight bw;
			ZeroMemory(&bw, sizeof(bw));

			if (type == IGameSkin::IGAME_RIGID_BLENDED)
			{
				for (int b = 0; b < Skin->GetNumberOfBones(i) && b < 4; b++)
				{
					INode* bone = Skin->GetBone(i, b);
					float weight = Skin->GetWeight(i, b);

					if (bone)
					{
						bw.id[b] = (byte)GetBoneID(bone->GetName());
						if (b < 3)
						{
							bw.weight[b] = weight;
						}
					}
				}
			}
			else if (type == IGameSkin::IGAME_RIGID)
			{
				INode * bone = Skin->GetBone(i, 0);

				if (bone)
				{
					bw.id[0] = (byte)GetBoneID(bone->GetName());
					bw.weight[0] = 1;
				}
			}

			mMaxMesh.BW[i] = bw;
		}

	Exit0:
		;
	}

	uint MeshExtracter::GetBoneID(const TCHAR * name)
	{
		return Skeleton::Instance().GetBoneId(name);
	}

	void MeshExtracter::ExtractFace(Face& face, const FaceEx* src)
	{
		for (size_t i = 0; i < 3; ++i)
		{
			face.vert[i] = src->vert[i];
			face.norm[i] = src->norm[i];
			face.uv[i] = src->texCoord[i];
		}
	}

	void MeshExtracter::ExtractMat(SubMesh * subMesh, IGameMaterial * gMaterial)
	{
		int texsCount = gMaterial->GetNumberOfTextureMaps();
		for (int i = 0; i < texsCount; i++)
		{
			IGameTextureMap* tex = gMaterial->GetIGameTextureMap(i);
			int texSlot = tex->GetStdMapSlot();
			const TCHAR* texFileName = tex->GetBitmapFileName();
			TSTR path, file, ext;
			SplitFilename(TSTR(texFileName), &path, &file, &ext);
			file += ext;
			switch (texSlot)
			{
			case ID_DI:
				subMesh->DiffTex = file;
				break;
			case ID_BU:
				subMesh->BumpTex = file;
				break;
			case ID_SP:
				subMesh->SpecTex = file;
				break;
			}
		}
	}

	void MeshExtracter::GetData(Model* model)
	{
		size_t count = mSubMeshes.size();
		for (size_t i = 0; i < count; ++i)
		{
			GetSubMeshData(model, mSubMeshes[i]);
		}
	}

	struct VertID 
	{
		uint vert;
		uint uv;
		uint norm;
	};
	
	struct VertIDComparer : public std::binary_function<VertID, VertID, bool>
	{
		bool operator() (const VertID& a, const VertID& b) const
		{
			if (a.vert < b.vert) return true;
			if (a.vert > b.vert) return false;

			if (a.uv < b.uv) return true;
			if (a.uv > b.uv) return false;

			if (a.norm < b.norm) return true;
			if (a.norm > b.norm) return false;

			return false;
		}
	};

	void MeshExtracter::GetSubMeshData(Model* model, const SubMesh* subMesh)
	{
		VertID vid;
		Vertex vert;
		Mesh* mesh = new Mesh();
		typedef std::map<VertID, uint, VertIDComparer> VertMap;
		VertMap vertMap;
		VertMap::iterator iter;

		mesh->name = subMesh->name;
		mesh->DiffTex = subMesh->DiffTex;
		mesh->BumpTex = subMesh->BumpTex;
		mesh->SpecTex = subMesh->SpecTex;
		mesh->vertexChannels = subMesh->vertexChannels;

		size_t faceCount = subMesh->faces.size();
		mesh->vertices.reserve(faceCount * 3);

		for (size_t i = 0; i < faceCount; ++i)
		{
			const Face& face = subMesh->faces[i];
			for (size_t j = 0; j < 3; ++j)
			{
				uint index = 0;
				vid.vert = face.vert[j];
				vid.norm = face.norm[j];
				vid.uv = face.uv[j];
				iter = vertMap.find(vid);
				if (iter == vertMap.end())
				{
					index = (uint)mesh->vertices.size();
					vert.pos = mMaxMesh.P[vid.vert];
					if (subMesh->vertexChannels & VertexChannel::BoneWeights)
						vert.bw = mMaxMesh.BW[vid.vert];
					vert.norm = mMaxMesh.N[vid.norm];
					vert.uv0 = mMaxMesh.UV[0][vid.uv];
					if (subMesh->vertexChannels & VertexChannel::TexCoords1)
						vert.uv1 = mMaxMesh.UV[1][vid.uv];
					mesh->vertices.push_back(vert);
					vertMap.insert(std::make_pair(vid, index));
				}
				else
				{
					index = iter->second;
				}

				mesh->indices.push_back(index);
			}
		}

		model->AddMesh(mesh);
	}
}
