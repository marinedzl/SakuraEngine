#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "Model.h"

namespace ModelFile
{
	Model::Model()
	{
	}

	Model::~Model()
	{
		DeleteList(mMeshes);
		DeleteMap(mMaterials);
	}

	Mesh* Model::AddMesh(const TCHAR* name)
	{
		Mesh* mesh = new Mesh();
		mesh->name = name;
		mMeshes.push_back(mesh);
		return mesh;
	}

	Material* Model::AddMaterial(void* id)
	{
		Material* mtl = new Material();
		mtl->id = id;
		mMaterials.insert(std::make_pair(id, mtl));
		return mtl;
	}

	Material* Model::GetMaterial(void* id)
	{
		Materials::iterator iter = mMaterials.find(id);
		return iter != mMaterials.end() ? iter->second : nullptr;
	}

	void WriteMesh(const TCHAR* path, const Mesh* mesh)
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
			block.size = sizeof(Vector3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::Normal)
		{
			block.type = MeshFile::Block::Normal;
			block.size = sizeof(Vector3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::Tangent)
		{
			block.type = MeshFile::Block::Tangent;
			block.size = sizeof(Vector3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::TexCoords0)
		{
			block.type = MeshFile::Block::TexCoords0;
			block.size = sizeof(Vector2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::TexCoords1)
		{
			block.type = MeshFile::Block::TexCoords1;
			block.size = sizeof(Vector2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (mesh->vertexChannels & VertexChannel::TexCoords2)
		{
			block.type = MeshFile::Block::TexCoords2;
			block.size = sizeof(Vector2) * head.vertexCount;
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
			}
		}
	Exit0:
		if (file)
			fclose(file);
	}

	void WriteJsonToFile(const Json::Value& value, const TCHAR* filename)
	{
		Json::StyledWriter writer;
		std::string buff = writer.write(value);
		FILE* file = nullptr;
		_wfopen_s(&file, filename, _T("w"));
		CHECK(file);
		fwrite(buff.c_str(), 1, buff.length(), file);
	Exit0:
		if (file)
			fclose(file);
	}

	DWORD ConverColor2Dword(const Vector3& color)
	{
		static const float factor = 255.0f;
		int r = (int)(color.x * factor);
		int g = (int)(color.y * factor);
		int b = (int)(color.z * factor);
		return r << 16 | g << 8 | b;
	}

	void AddColorNode(Json::Value& mtlNode, const char* name, const Vector3& src)
	{
		char buff[32];
		DWORD color = ConverColor2Dword(src);
		_ultoa_s(color, buff, 16);
		mtlNode[name] = buff;
	}

	void AddFloatNode(Json::Value& mtlNode, const char* name, float src)
	{
		double d = (int)(src * 1000);
		d /= 1000;
		mtlNode[name] = d;
	}

	void WriteMaterial(Json::Value& mtlNode, const Material* material, const TString& path)
	{
		mtlNode["Shader"] = "Normal-Diffuse";
		for (size_t i = 0; i < material->textures.size(); i++)
		{
			std::map<std::string, TString>::const_iterator iter = material->textures.begin();
			std::map<std::string, TString>::const_iterator iterEnd = material->textures.end();
			for (; iter != iterEnd; ++iter)
			{
				if (iter->first == "Opacity")
				{
					mtlNode["Shader"] = "AlphaTest-Diffuse";
				}
				else if (iter->first == "Diffuse")
				{
					mtlNode["_MainTex"] = WStr2MStr(path + iter->second);
				}
				else if (iter->first == "Bump")
				{
					mtlNode["_BumpMap"] = WStr2MStr(path + iter->second);
				}
			}
		}
	}

	void Model::WriteFile(const TCHAR* path, const TCHAR* name)
	{
		Json::Value root;
		Json::Value entity;
		TString strFullPath = path;
		TString strPath = name;

		strPath += _T("\\");
		strFullPath += _T("\\");

		strFullPath += name;
		CreateMultiDir(strFullPath.c_str());
		strFullPath += _T("\\");

		entity["Name"] = WStr2MStr(name);

		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			Mesh* mesh = mMeshes[i];
			Json::Value value;
			value["Mesh"] = WStr2MStr(strPath + mesh->name + _T(".mesh"));
			if (Material* material = GetMaterial(mesh->mtlid))
			{
				Json::Value mtl;
				WriteMaterial(mtl, material, strPath);
				value["Material"] = mtl;
			}
			entity["Renderer"].append(value);
			WriteMesh(strFullPath.c_str(), mMeshes[i]);
		}

		root["Entities"].append(entity);

		strFullPath = path;
		strFullPath += _T("\\");
		strFullPath += name;
		strFullPath += _T(".mdl");
		WriteJsonToFile(root, strFullPath.c_str());
	}

	void Model::OptMesh()
	{
		size_t meshCount = mMeshes.size();
		for (size_t i = 0; i < meshCount; i++)
		{
			Mesh* mesh = mMeshes[i];
			size_t vertexCount = mesh->vertices.size();
			mesh->indices.reserve(vertexCount);
			for (size_t j = 0; j < vertexCount; j++)
			{
				mesh->indices.push_back(j);
			}
		}
	}
}
