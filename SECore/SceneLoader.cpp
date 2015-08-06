#include "stdafx.h"
#include "mesh.h"
#include "Texture.h"
#include "Scene.h"
#include "Skeleton.h"
#include "Animation.h"
#include "AnimationClip.h"
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

SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
}

Collider::Type String2ColliderShape(const char* typeName)
{
	if (strcmp(typeName, "Box") == 0)
	{
		return Collider::eBox;
	}
	return Collider::eInvalid;
}

Vector3 Json2Vec3(const Json::Value& value)
{
	Vector3 ret;
	ret.x = (float)value[(size_t)0].asDouble();
	ret.y = (float)value[1].asDouble();
	ret.z = (float)value[2].asDouble();
	return ret;
}

Quat Json2Quat(const Json::Value& value)
{
	Quat ret;
	ret.x = (float)value[(size_t)0].asDouble();
	ret.y = (float)value[1].asDouble();
	ret.z = (float)value[2].asDouble();
	ret.w = (float)value[3].asDouble();
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
		Scene::Entity* entity = scene->CreateEntity();
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
				position = Json2Vec3(transformRoot["Position"]);

			if (transformRoot.isMember("Rotation"))
				rotation = Json2Quat(transformRoot["Rotation"]);

			if (transformRoot.isMember("Scaling"))
				scaling = Json2Vec3(transformRoot["Scaling"]);

			AffineTransform(mat, position, rotation, scaling);
			entity->SetWorld(mat);
		}

		if (entityRoot.isMember("Animation"))
		{
			const Json::Value& animationRoot = entityRoot["Animation"];

			Animation* animation = dynamic_cast<Animation*>(entity->CreateAnimation());

			Skeleton* skeleton = gResourceManager.LoadSkeleton(animationRoot["Skeleton"].asCString());
			animation->SetSkeleton(skeleton);
			skeleton->Release();

			const Json::Value& jsonClips = animationRoot["Clips"];
			std::vector<std::string> memberNames = jsonClips.getMemberNames();
			for (size_t i = 0; i < memberNames.size(); ++i)
			{
				AnimationClip* clip = gResourceManager.LoadClip(jsonClips[memberNames[i]].asCString());
				animation->AddClip(memberNames[i].c_str(), clip);
				clip->Release();
			}

			if (animationRoot.isMember("Clip"))
			{
				animation->Play(animationRoot["Clip"].asCString());
			}
		}

		if (entityRoot.isMember("Collider"))
		{
			const Json::Value& colliderRoot = entityRoot["Collider"];
			Collider::Type type = String2ColliderShape(colliderRoot["Shape"].asCString());
			Collider* collider = entity->CreateCollider(type);
			switch (type)
			{
			case Collider::eBox:
			{
				SECore::BoxCollider* boxCollider = dynamic_cast<SECore::BoxCollider*>(collider);
				CHECK(boxCollider);
				if (colliderRoot.isMember("Size"))
					boxCollider->SetSize(Json2Vec3(colliderRoot["Size"]));
			}
				break;
			}
		}


		if (entityRoot.isMember("Renderer"))
		{
			const Json::Value& rendererRoot = entityRoot["Renderer"];
			CHECK(rendererRoot.isArray());
			size_t count = rendererRoot.size();
			SECore::Renderer* renderer = entity->CreateRenderer();
			for (size_t i = 0; i < count; ++i)
			{
				SECore::Renderer::Entity* re = renderer->CreateEntity();
				const Json::Value& reRoot = rendererRoot[i];
				if (reRoot.isMember("Mesh"))
				{
					Mesh* mesh = gResourceManager.LoadMesh(reRoot["Mesh"].asCString());
					re->SetMesh(mesh);
					mesh->Release();
				}
				if (reRoot.isMember("Material"))
				{
					SECore::Material* material = re->GetMaterial();
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
							Texture* texture = gResourceManager.LoadTexture(mtlRoot[propName].asCString());
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
