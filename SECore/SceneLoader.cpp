#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "SceneLoader.h"

DWORD ConverColor2Dword(const Color& color)
{
	static const float factor = 255.0f;
	int a = (int)(color.a * factor);
	int b = (int)(color.b * factor);
	int g = (int)(color.g * factor);
	int r = (int)(color.r * factor);
	return r << 24 | g << 16 | b << 8 | a;
}

Color ConvertDwordToColor(DWORD dw)
{
	static const float factor = 1 / 255.0f;
	int a = dw & 255;
	int b = dw >> 8 & 255;
	int g = dw >> 16 & 255;
	int r = dw >> 24 & 255;
	return Color(r * factor, g * factor, b * factor, a * factor);
}

SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
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
						shader = gResourceManager.CreateShader(mtlRoot["Shader"].asCString());
					}
					else
					{
						shader = gResourceManager.CreateShader("AlphaTest-Diffuse");
					}

					CHECK(shader);

					material->SetShader(shader);

					std::vector<std::string> propNames = mtlRoot.getMemberNames();
					size_t propCount = propNames.size();
					for (size_t pi = 0; pi < propCount; pi++)
					{
						const std::string& propName = propNames[pi];
						Shader::PropertyType propType = shader->GetPropertyType(propName.c_str());
						switch (propType)
						{
						case Shader::eFloat:
						{
							material->SetFloat(propName.c_str(), (float)mtlRoot[propName].asDouble());
						}
							break;
						case Shader::eColor:
						{
							const char* str = mtlRoot[propName].asCString();
							DWORD dwColor = strtoul(str, NULL, 16);
							Color color = ConvertDwordToColor(dwColor);
							material->SetColor(propName.c_str(), color);
						}
							break;
						case Shader::eTexture:
						{
							ITexture* texture = gResourceManager.LoadTexture(mtlRoot[propName].asCString());
							material->SetTexture(propName.c_str(), texture);
							texture->Release();
						}
							break;
						default:
							CHECK(false);
							break;
						}
					}
				}
			}
		}
	}

	ret = true;
Exit0:
	return ret;
}
