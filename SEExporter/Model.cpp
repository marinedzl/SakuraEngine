#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "MeshExtracter.h"
#include "Skeleton.h"
#include "Model.h"

namespace MaxPlugin
{
	Model::Model()
	{
	}

	Model::~Model()
	{
		Clear();
	}

	void Model::Clear()
	{
		DeleteList(mMeshes);
	}

	bool Model::Extract(IGameScene* gScene)
	{
		bool ret = false;
		CHECK(gScene);
		Clear();

		mMeshExtracter = new MeshExtracter();

		int count = gScene->GetTopLevelNodeCount();
		for (int i = 0; i < count; ++i)
			ExtractNode(gScene->GetTopLevelNode(i));

		mMeshExtracter->Clear();
		delete mMeshExtracter;

		ret = true;
	Exit0:
		return ret;
	}

	void Model::ExtractNode(IGameNode* gNode)
	{
		if (gNode->GetIGameObject()->GetIGameType() == IGameObject::IGAME_MESH)
		{
			mMeshExtracter->Extract(gNode);
			mMeshExtracter->GetData(this);
			mMeshExtracter->Clear();
		}

		for (int i = 0; i < gNode->GetChildCount(); i++)
		{
			ExtractNode(gNode->GetNodeChild(i));
		}
	}

	void Model::WriteMesh(const Mesh* mesh, const TCHAR* path)
	{
		MeshFile::Head head;
		MeshFile::Block block;
		std::vector<MeshFile::Block> blocks;
		TString filename;
		FILE* file = nullptr;

		filename = path;
		filename += mesh->name;
		filename += _T(".mesh");
		_wfopen_s(&file, filename.c_str(), _T("wb"));
		CHECK(file);

		head.version = 100;
		head.indexCount = (UINT)mesh->indices.size();
		head.vertexCount = (UINT)mesh->vertices.size();

		block.type = MeshFile::Block::Face;
		block.size = sizeof(USHORT) * head.indexCount;
		blocks.push_back(block);

		if (mesh->vertexChannels & VertexChannel::Position)
		{
			block.type = MeshFile::Block::Position;
			block.size = sizeof(Vec3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::Normal)
		{
			block.type = MeshFile::Block::Normal;
			block.size = sizeof(Vec3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::Tangent)
		{
			block.type = MeshFile::Block::Tangent;
			block.size = sizeof(Vec3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::TexCoords0)
		{
			block.type = MeshFile::Block::TexCoords0;
			block.size = sizeof(Vec2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::TexCoords1)
		{
			block.type = MeshFile::Block::TexCoords1;
			block.size = sizeof(Vec2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::TexCoords2)
		{
			block.type = MeshFile::Block::TexCoords2;
			block.size = sizeof(Vec2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::BoneWeights)
		{
			block.type = MeshFile::Block::BoneWeights;
			block.size = sizeof(BoneWeight) * head.vertexCount;
			blocks.push_back(block);
		}

		head.blockCount = (UINT)blocks.size();

		fwrite(&head, sizeof(MeshFile::Head), 1, file);
		fwrite(&blocks[0], sizeof(MeshFile::Block), head.blockCount, file);

		fwrite(&mesh->indices[0], sizeof(USHORT), head.indexCount, file);

		for (size_t i = 0; i < head.blockCount; ++i)
		{
			switch (blocks[i].type)
			{
			case MeshFile::Block::Position:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].pos, sizeof(mesh->vertices[j].pos), 1, file);
			}
			break;
			case MeshFile::Block::Normal:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].norm, sizeof(mesh->vertices[j].norm), 1, file);
			}
			break;
			case MeshFile::Block::Tangent:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].tan, sizeof(mesh->vertices[j].tan), 1, file);
			}
			break;
			case MeshFile::Block::TexCoords0:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].uv0, sizeof(mesh->vertices[j].uv0), 1, file);
			}
			break;
			case MeshFile::Block::TexCoords1:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].uv1, sizeof(mesh->vertices[j].uv1), 1, file);
			}
			break;
			case MeshFile::Block::TexCoords2:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].uv2, sizeof(mesh->vertices[j].uv2), 1, file);
			}
			break;
			case MeshFile::Block::BoneWeights:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].bw, sizeof(mesh->vertices[j].bw), 1, file);
			}
			break;
			}
		}
	Exit0:
		if (file)
			fclose(file);
	}

	void Model::WriteFile(const TCHAR* filename)
	{
		TSTR path, name, ext;
		SplitFilename(TSTR(filename), &path, &name, &ext);

		Json::Value root;
		Json::Value entity;
		TString strFullPath = (const TCHAR*)path;
		TString strPath = (const TCHAR*)name;

		strPath += _T("\\");
		strFullPath += _T("\\");

		strFullPath += name;
		CreateMultiDir(strFullPath.c_str());
		strFullPath += _T("\\");

		entity["Name"] = WStr2MStr((const TCHAR*)name);

		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			const Mesh* mesh = mMeshes[i];

			Json::Value value;
			value["Mesh"] = WStr2MStr(strPath + mesh->name + _T(".mesh"));

			if (!mesh->DiffTex.empty())
				value["Material"]["DiffTex"] = WStr2MStr(strPath + mesh->DiffTex);

			if (!mesh->BumpTex.empty())
				value["Material"]["BumpTex"] = WStr2MStr(strPath + mesh->BumpTex);

			if (!mesh->SpecTex.empty())
				value["Material"]["SpecTex"] = WStr2MStr(strPath + mesh->SpecTex);

			entity["Renderer"].append(value);

			WriteMesh(mMeshes[i], strFullPath.c_str());
		}

		root["Entities"].append(entity);

		strFullPath = path;
		strFullPath += _T("\\");
		strFullPath += name;
		strFullPath += _T(".mdl");
		SaveJsonToFile(strFullPath.c_str(), root);
	}
}
