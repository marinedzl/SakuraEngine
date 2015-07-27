#pragma once

struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

namespace ModelFile
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
		};
	};

	struct Vertex
	{
		Vector3 pos;
		Vector3 norm;
		Vector3 tan;
		Vector2 uv0;
		Vector2 uv1;
		Vector2 uv2;
	};

	class Mesh
	{
	public:
		TString name;
		TString mtlName;
		UINT vertexChannels;
		std::vector<Vertex> vertices;
	};

	class Material
	{
	public:
		TString name;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		Vector3 emissive;
		float opacity;
		float glossiness;
		float specularLevel;
		float emissiveAmount;
		std::map<std::string, TString> textures;
	};

	class Model
	{
	public:
		Model();
		virtual ~Model();
		Mesh* AddMesh(const TCHAR* name);
		Material* AddMaterial(const TCHAR* name);
		Material* GetMaterial(const TCHAR* name);
		void WriteFile(const TCHAR* path, const TCHAR* name);
	private:
		typedef std::map<TString, Material*> Materials;
	private:
		std::vector<Mesh*> mMeshes;
		Materials mMaterials;
	};
}
