#include "stdafx.h"
#include <json\json.h>
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

	Material* Model::AddMaterial(const TCHAR* name)
	{
		Material* mtl = new Material();
		mtl->name = name;
		mMaterials.insert(std::make_pair(name, mtl));
		return mtl;
	}

	Material* Model::GetMaterial(const TCHAR* name)
	{
		Materials::iterator iter = mMaterials.find(name);
		return iter != mMaterials.end() ? iter->second : nullptr;
	}

	void WriteMesh(const TCHAR* path, const Mesh* mesh)
	{
		struct Head
		{
			UINT vertexChannels;
			UINT vertexCount;
			UINT blockCount;
		};

		struct Block
		{
			UINT type;
			UINT size;
		};

		Head head;
		Block block;
		std::vector<Block> blocks;
		TString filename;
		FILE* file = nullptr;
		
		filename = path;
		filename += mesh->name;
		filename += _T(".mesh");
		_wfopen_s(&file, filename.c_str(), _T("wb"));
		CHECK(file);

		head.vertexChannels = (UINT)mesh->vertexChannels;
		head.vertexCount = (UINT)mesh->vertices.size();

		if (head.vertexChannels & VertexChannel::Position)
		{
			block.type = VertexChannel::Position;
			block.size = sizeof(Vector3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (head.vertexChannels & VertexChannel::Normal)
		{
			block.type = VertexChannel::Normal;
			block.size = sizeof(Vector3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (head.vertexChannels & VertexChannel::Tangent)
		{
			block.type = VertexChannel::Tangent;
			block.size = sizeof(Vector3) * head.vertexCount;
			blocks.push_back(block);
		}

		if (head.vertexChannels & VertexChannel::TexCoords0)
		{
			block.type = VertexChannel::TexCoords0;
			block.size = sizeof(Vector2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (head.vertexChannels & VertexChannel::TexCoords1)
		{
			block.type = VertexChannel::TexCoords1;
			block.size = sizeof(Vector2) * head.vertexCount;
			blocks.push_back(block);
		}

		if (head.vertexChannels & VertexChannel::TexCoords2)
		{
			block.type = VertexChannel::TexCoords2;
			block.size = sizeof(Vector2) * head.vertexCount;
			blocks.push_back(block);
		}

		head.blockCount = (UINT)blocks.size();

		fwrite(&head, sizeof(Head), 1, file);
		fwrite(&blocks[0], sizeof(Block), head.blockCount, file);

		for (size_t i = 0; i < head.blockCount; ++i)
		{
			switch (block.type)
			{
			case VertexChannel::Position:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].pos, sizeof(mesh->vertices[j].pos), 1, file);
			}
				break;
			case VertexChannel::Normal:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].norm, sizeof(mesh->vertices[j].norm), 1, file);
			}
			break;
			case VertexChannel::Tangent:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].tan, sizeof(mesh->vertices[j].tan), 1, file);
			}
			break;
			case VertexChannel::TexCoords0:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].uv0, sizeof(mesh->vertices[j].uv0), 1, file);
			}
			break;
			case VertexChannel::TexCoords1:
			{
				for (size_t j = 0; j < head.vertexCount; ++j)
					fwrite(&mesh->vertices[j].uv1, sizeof(mesh->vertices[j].uv1), 1, file);
			}
			break;
			case VertexChannel::TexCoords2:
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
	/*
	Color ConvertDwordToColor(DWORD dw)
	{
		static const float factor = 1 / 255.0f;
		int a = dw & 255;
		int b = dw >> 8 & 255;
		int g = dw >> 16 & 255;
		int r = dw >> 24 & 255;
		return Color(r * factor, g * factor, b * factor, a * factor);
	}*/

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

	void WriteMaterial(Json::Value& mtlNode, const Material* material)
	{
		for (size_t i = 0; i < material->textures.size(); i++)
		{
			std::map<std::string, TString>::const_iterator iter = material->textures.begin();
			std::map<std::string, TString>::const_iterator iterEnd = material->textures.end();
			for (; iter != iterEnd; ++iter)
			{
				mtlNode["Textures"][iter->first] = WStr2MStr(iter->second);
				AddColorNode(mtlNode, "Ambient", material->ambient);
				AddColorNode(mtlNode, "Diffuse", material->diffuse);
				AddColorNode(mtlNode, "Specular", material->specular);
				AddColorNode(mtlNode, "Emissive", material->emissive);
				AddFloatNode(mtlNode, "Opacity", material->opacity);
				AddFloatNode(mtlNode, "Glossiness", material->glossiness);
				AddFloatNode(mtlNode, "SpecularLevel", material->specularLevel);
				AddFloatNode(mtlNode, "EmissiveAmount", material->emissiveAmount);
			}
		}
	}

	void Model::WriteFile(const TCHAR* path, const TCHAR* name)
	{
		Json::Value root;
		TString filename = path;

		filename += _T("\\");

		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			Mesh* mesh = mMeshes[i];
			Json::Value value;
			value["Mesh"] = WStr2MStr(mesh->name);
			if (Material* material = GetMaterial(mesh->mtlName.c_str()))
			{
				Json::Value mtl;
				WriteMaterial(mtl, material);
				value["Material"] = mtl;
			}
			root.append(value);
			WriteMesh(filename.c_str(), mMeshes[i]);
		}

		filename += name;
		filename += _T(".mdl");
		WriteJsonToFile(root, filename.c_str());
	}
}
