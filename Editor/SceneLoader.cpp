#include "stdafx.h"
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

bool LoadJsonFromFile(const TCHAR* filename, Json::Value& root)
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

Vector3 Json2Vec3(const Json::Value& value)
{
	Vector3 ret;
	ret.x = (float)value[(size_t)0].asDouble();
	ret.y = (float)value[1].asDouble();
	ret.z = (float)value[2].asDouble();
	return ret;
}

Color Json2Color(const Json::Value& value)
{
	Color ret;
	ret.r = (float)value[(size_t)0].asDouble();
	ret.g = (float)value[1].asDouble();
	ret.b = (float)value[2].asDouble();
	ret.a = (float)value[3].asDouble();
	return ret;
}

const float Angle2Radian = XM_PI / 180.0f;

Quat Json2Quat(const Json::Value& value)
{
	Quat ret;
	Vector3 v3;
	v3.x = (float)value[(size_t)0].asDouble();
	v3.y = (float)value[1].asDouble();
	v3.z = (float)value[2].asDouble();
	XMVECTOR q = XMQuaternionRotationRollPitchYaw(v3.x * Angle2Radian, v3.y * Angle2Radian, v3.z * Angle2Radian);
	XMStoreFloat4((XMFLOAT4*)&ret, q);
	return ret;
}

void LoadTransform(Transform& transform, const Json::Value& transformRoot)
{
	if (transformRoot.isMember("Position"))
		transform.position = Json2Vec3(transformRoot["Position"]);

	if (transformRoot.isMember("Rotation"))
		transform.rotation = Json2Quat(transformRoot["Rotation"]);

	if (transformRoot.isMember("Scaling"))
		transform.scaling = Json2Vec3(transformRoot["Scaling"]);
}

bool LoadEntity(SECore::Core* core, SECore::Scene::Entity* entity, const Json::Value& entityRoot)
{
	bool ret = false;

	if (entityRoot.isMember("Name"))
	{
		entity->SetName(entityRoot["Name"].asCString());
	}

	if (entityRoot.isMember("Transform"))
		LoadTransform(entity->GetTransform(), entityRoot["Transform"]);

	if (entityRoot.isMember("Animation"))
	{
		const Json::Value& animationRoot = entityRoot["Animation"];

		SECore::Animation* animation = entity->GetAnimation();

		if (!animation)
			animation = entity->CreateAnimation();

		SECore::Skeleton* skeleton = core->LoadSkeleton(animationRoot["Skeleton"].asCString());
		animation->SetSkeleton(skeleton);
		skeleton->Release();

		if (animationRoot.isMember("SavedBones"))
		{
			const Json::Value& savedBonesRoot = animationRoot["SavedBones"];
			size_t count = savedBonesRoot.size();
			for (size_t i = 0; i < count; i++)
			{
				animation->AddSavedBoneTM(savedBonesRoot[i].asCString());
			}
		}

		const Json::Value& jsonClips = animationRoot["Clips"];
		std::vector<std::string> memberNames = jsonClips.getMemberNames();
		for (size_t i = 0; i < memberNames.size(); ++i)
		{
			SECore::AnimationClip* clip = core->LoadClip(jsonClips[memberNames[i]].asCString());
			clip->SetName(memberNames[i].c_str());
			animation->AddClip(clip);
			clip->Release();
		}
	}

	if (entityRoot.isMember("Renderer"))
	{
		const Json::Value& rendererRoot = entityRoot["Renderer"];

		CHECK(rendererRoot.isArray());
		size_t count = rendererRoot.size();
		SECore::Renderer* renderer = entity->GetRenderer();

		if (!renderer)
		{
			renderer = entity->CreateRenderer();
		}

		for (size_t i = 0; i < count; ++i)
		{
			SECore::Renderer::Entity* re = renderer->CreateEntity();
			const Json::Value& reRoot = rendererRoot[i];
			if (reRoot.isMember("CastShadow"))
			{
				re->SetCastShadow(reRoot["CastShadow"].asBool());
			}
			if (reRoot.isMember("Mesh"))
			{
				SECore::Mesh* mesh = core->LoadMesh(reRoot["Mesh"].asCString());
				re->SetMesh(mesh);
				mesh->Release();
			}
			if (reRoot.isMember("Material"))
			{
				SECore::Material* material = re->GetMaterial();
				const Json::Value& mtlRoot = reRoot["Material"];

				SECore::Shader* shader = nullptr;
				if (mtlRoot.isMember("Shader"))
				{
					shader = core->CreateShader(mtlRoot["Shader"].asCString());
				}
				else
				{
					shader = core->CreateShader("AlphaTest-Diffuse");
				}

				CHECK(shader);

				material->SetShader(shader);

				std::vector<std::string> propNames = mtlRoot.getMemberNames();
				size_t propCount = propNames.size();
				for (size_t pi = 0; pi < propCount; pi++)
				{
					const std::string& propName = propNames[pi];
					SECore::Shader::PropertyType propType = shader->GetPropertyType(propName.c_str());
					switch (propType)
					{
					case SECore::Shader::eFloat:
					{
						material->SetFloat(propName.c_str(), (float)mtlRoot[propName].asDouble());
					}
					break;
					case SECore::Shader::eColor:
					{
						const char* str = mtlRoot[propName].asCString();
						DWORD dwColor = strtoul(str, NULL, 16);
						Color color = ConvertDwordToColor(dwColor);
						material->SetColor(propName.c_str(), color);
					}
					break;
					case SECore::Shader::eTexture:
					{
						SECore::Texture* texture = core->LoadTexture(mtlRoot[propName].asCString());
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

		entity->CalcBound();
	}

	if (entityRoot.isMember("Collider"))
	{
		const Json::Value& colliderRoot = entityRoot["Collider"];

		bool isDynamic = false;

		if (colliderRoot.isMember("Dynamic"))
		{
			isDynamic = colliderRoot["Dynamic"].asBool();
		}

		SECore::Collider* collider = nullptr;

		if (strcmp(colliderRoot["Shape"].asCString(), "Box") == 0)
		{
			Vector3 center, size;

			SECore::Bound bound = entity->GetBound();
			bound.max = bound.max * entity->GetTransform().scaling;
			bound.min = bound.min * entity->GetTransform().scaling;

			size = (bound.max - bound.min) / 2;
			center = bound.min + size;

			if (colliderRoot.isMember("Size"))
				size = Json2Vec3(colliderRoot["Size"]);

			if (colliderRoot.isMember("Offset"))
				size = Json2Vec3(colliderRoot["Offset"]);

			collider = entity->CreateBoxCollider(isDynamic, size);
			collider->SetLocalPose(center, Quat());
		}

		if (isDynamic)
		{
			SECore::RigidBody* rigidBody = collider->GetRigidBody();
			CHECK(rigidBody);

			bool enableGravity = false;
			if (colliderRoot.isMember("Gravity"))
			{
				enableGravity = colliderRoot["Gravity"].asBool();
			}

			rigidBody->EnableGravity(enableGravity);

			if (colliderRoot.isMember("Mass"))
			{
				rigidBody->SetMass((float)colliderRoot["Mass"].asDouble());
			}
		}
	}

	if (entityRoot.isMember("CharacterController"))
	{
		const Json::Value& cctRoot = entityRoot["CharacterController"];
		Vector3 offset, size;
		float height, radius;

		SECore::Bound bound = entity->GetBound();
		bound.max = bound.max * entity->GetTransform().scaling;
		bound.min = bound.min * entity->GetTransform().scaling;

		size = (bound.max - bound.min) / 2;
		offset = bound.min + size;

		height = size.y;
		radius = (size.x + size.z) / 2;

		if (cctRoot.isMember("Height"))
			height = (float)cctRoot["Height"].asDouble();

		if (cctRoot.isMember("Radius"))
			radius = (float)cctRoot["Radius"].asDouble();

		if (cctRoot.isMember("Offset"))
			offset = Json2Vec3(cctRoot["Offset"]);

		entity->CreateCCT(height, radius, offset);
	}

	ret = true;
Exit0:
	return ret;
}

bool LoadLight(SECore::Scene* scene, const Json::Value& lightRoot)
{
	const char* type = lightRoot["Type"].asCString();

	SECore::Light* light = nullptr;

	if (strcmp(type, "Point") == 0)
	{
		light = scene->AddPointLight();
	}
	else if (strcmp(type, "Directional") == 0)
	{
		light = scene->AddDirectionalLight();
	}

	if (lightRoot.isMember("Enable"))
	{
		light->Enable(lightRoot["Enable"].asBool());
	}

	if (lightRoot.isMember("Name"))
	{
		light->SetName(lightRoot["Name"].asCString());
	}

	if (lightRoot.isMember("Transform"))
		LoadTransform(light->GetTransform(), lightRoot["Transform"]);

	if (lightRoot.isMember("Color"))
		light->SetColor(Json2Color(lightRoot["Color"]));

	if (lightRoot.isMember("Intensity"))
		light->SetIntensity((float)(lightRoot["Intensity"].asDouble()));

	return true;
}

bool SceneLoader::Load(SECore::Scene* scene, const TCHAR* filename)
{
	bool ret = false;
	Json::Value root;
	CHECK(scene);
	CHECK(LoadJsonFromFile(filename, root));

	if (root.isMember("Config"))
	{
		const Json::Value& configRoot = root["Config"];
		if (configRoot.isMember("ShowGizmo"))
			scene->GetConfig()->EnableGizmo(configRoot["ShowGizmo"].asBool());
		if (configRoot.isMember("AmbientColor"))
		{
			scene->GetConfig()->SetAmbientColor(Json2Color(configRoot["AmbientColor"]));
		}
	}

	CHECK(root.isMember("Entities"));
	CHECK(root["Entities"].isArray());

	size_t entityCount = root["Entities"].size();

	for (size_t i = 0; i < entityCount; ++i)
	{
		SECore::Scene::Entity* entity = scene->CreateEntity();
		const Json::Value& entityRoot = root["Entities"][i];

		if (entityRoot.isMember("Prefab"))
		{
			CString filename = MStr2WStr(entityRoot["Prefab"].asCString());
			Json::Value prefab;
			LoadJsonFromFile(filename, prefab);
			LoadEntity(scene->GetCore(), entity, prefab);
		}
		
		LoadEntity(scene->GetCore(), entity, entityRoot);
	}

	size_t lightCount = root["Lights"].size();
	for (size_t i = 0; i < lightCount; i++)
	{
		LoadLight(scene, root["Lights"][i]);
	}

	ret = true;
Exit0:
	return ret;
}
