#include "stdafx.h"
#include "Scene.h"
#include "Material.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SceneLoader.h"

SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
}

bool LoadJsonFromFile(const char* filename, Json::Value& root)
{
	bool ret = false;
	buffer file;
	Json::Reader reader;

	CHECK(LoadTextFile(file, filename) && file.size() > 0);

	if (!reader.parse(file.ptr(), root))
	{
		log(reader.getFormatedErrorMessages().c_str());
		CHECK(false);
	}

	ret = true;
Exit0:
	return ret;
}

bool SaveJsonToFile(const char* filename, const Json::Value& root)
{
	bool ret = false;
	FILE* file = nullptr;
	Json::StyledWriter writer;
	std::string dst;

	dst = writer.write(root);
	fopen_s(&file, filename, "w");
	CHECK(file);
	fwrite(dst.c_str(), 1, dst.length(), file);

	ret = true;
Exit0:
	if (file)
		fclose(file);
	return ret;
}

bool SceneLoader::Load(Scene* scene, const char* filename)
{
	bool ret = false;
	Json::Value root;
	CHECK(scene);
	CHECK(LoadJsonFromFile(filename, root));

	CHECK(root.isMember("Entities"));
	CHECK(root["Entities"].isArray());

	size_t entityCount = root["Entities"].size();

	for (size_t i = 0; i < entityCount; ++i)
	{
		Scene::Entity* entity = dynamic_cast<Scene::Entity*>(scene->CreateEntity());
		const Json::Value& entityRoot = root["Entities"][i];
		if (entityRoot.isMember("Name"))
		{
			entity->SetName(entityRoot["Name"].asCString());
		}
		if (entityRoot.isMember("Transform"))
		{
			Matrix mat;
			Vector3 position, scaling;
			Quat rotation;
			const Json::Value& transformRoot = entityRoot["Transform"];
			if (transformRoot.isMember("Position"))
			{
				CHECK(transformRoot["Position"].isArray() && transformRoot["Position"].size() >= 3);
				position.x = (float)transformRoot["Position"][size_t(0)].asDouble();
				position.y = (float)transformRoot["Position"][size_t(1)].asDouble();
				position.z = (float)transformRoot["Position"][size_t(2)].asDouble();
			}
			if (transformRoot.isMember("Rotation"))
			{
				CHECK(transformRoot["Rotation"].isArray() && transformRoot["Rotation"].size() >= 4);
				rotation.x = (float)transformRoot["Rotation"][size_t(0)].asDouble();
				rotation.y = (float)transformRoot["Rotation"][size_t(1)].asDouble();
				rotation.z = (float)transformRoot["Rotation"][size_t(2)].asDouble();
				rotation.w = (float)transformRoot["Rotation"][size_t(3)].asDouble();
			}
			if (transformRoot.isMember("Scaling"))
			{
				CHECK(transformRoot["Scaling"].isArray() && transformRoot["Scaling"].size() >= 3);
				scaling.x = (float)transformRoot["Scaling"][size_t(0)].asDouble();
				scaling.y = (float)transformRoot["Scaling"][size_t(1)].asDouble();
				scaling.z = (float)transformRoot["Scaling"][size_t(2)].asDouble();
			}
			AffineTransform(mat, position, rotation, scaling);
			entity->SetWorld(mat);
		}

		if (entityRoot.isMember("Renderer"))
		{
			const Json::Value& rendererRoot = entityRoot["Renderer"];
			CHECK(rendererRoot.isArray());
			size_t count = rendererRoot.size();
			IRenderer* renderer = entity->CreateRenderer();
			for (size_t i = 0; i < count; ++i)
			{
				IRenderer::Entity* re = renderer->CreateEntity();
				const Json::Value& reRoot = rendererRoot[i];
				if (reRoot.isMember("Mesh"))
				{
					IMesh* mesh = gResourceManager.LoadMesh(reRoot["Mesh"].asCString());
					re->SetMesh(mesh);
					mesh->Release();
				}
				if (reRoot.isMember("Material"))
				{
					Material* material = dynamic_cast<Material*>(re->GetMaterial());
					const Json::Value& mtlRoot = reRoot["Material"];

					Shader* shader = nullptr;
					if (mtlRoot.isMember("Shader"))
					{
						shader = dynamic_cast<Shader*>(gResourceManager.LoadShader(mtlRoot["Shader"].asCString()));
						material->SetShader(shader);
					}

					std::vector<std::string> propNames = mtlRoot.getMemberNames();
					size_t propCount = propNames.size();
					for (size_t pi = 0; pi < propCount; pi++)
					{
						const std::string& propName = propNames[pi];
						if (const Shader::Property* prop = shader->GetProperty(propName.c_str()))
						{
							const AnyValue& defaultValue = prop->GetValue();
							if (defaultValue.GetType() == eTexture2D)
							{
								ITexture* texture = gResourceManager.LoadTexture(mtlRoot[propName].asCString());
								material->SetTexture("_MainTex", texture);
								texture->Release();
							}
							else
							{
								AnyValue value;
								String2Value(mtlRoot[propName], defaultValue.GetType(), value);
							}
						}
					}

					if (shader)
						shader->Release();
				}
			}
		}
	}

	ret = true;
Exit0:
	return ret;
}
