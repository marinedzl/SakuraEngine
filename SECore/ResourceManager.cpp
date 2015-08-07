#include "stdafx.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshShader.h"
#include "Skeleton.h"
#include "AnimationClip.h"
#include "ResourceManager.h"

ResourceManager gResourceManager;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Release()
{
	DeleteMap(mShaderFactories);
	DeleteMap(mResources);
}

bool ResourceManager::Init()
{
	mShaderFactories.insert(std::make_pair(std::string("AlphaTest-Diffuse"), new MeshShaderFactory()));
	return true;
}

Texture* ResourceManager::LoadTexture(const char* name)
{
	return LoadResource<Texture>(name);
}

Mesh* ResourceManager::LoadMesh(const char* name)
{
	return LoadResource<Mesh>(name);
}

AnimationClip* ResourceManager::LoadClip(const char* name)
{
	return LoadResource<AnimationClip>(name);
}

Skeleton* ResourceManager::LoadSkeleton(const char* name)
{
	return LoadResource<Skeleton>(name);
}

bool ResourceManager::AddShaderFactory(const char * name, ShaderFactory * factory)
{
	mShaderFactories.insert(std::make_pair(name, factory));
	return true;
}

ShaderFactory * ResourceManager::FindShaderFactory(const char * name)
{
	ShaderFactories::iterator iter = mShaderFactories.find(name);
	return iter == mShaderFactories.end() ? nullptr : iter->second;
}

Shader * ResourceManager::CreateShader(const char * name)
{
	Shader* ret = nullptr;
	if (ShaderFactory* factory = FindShaderFactory(name))
	{
		ret = factory->CreateNew();
		if (ret)
		{
			if (!ret->Init())
			{
				ret->Release();
				ret = nullptr;
			}
		}
	}
	return ret;
}

template<typename T>
T* ResourceManager::LoadResource(const char* name)
{
	T* res = nullptr;
	Resources::iterator iter;

	CHECK(name);

	iter = mResources.find(name);
	if (iter != mResources.end())
	{
		res = dynamic_cast<T*>(iter->second);
	}
	else
	{
		res = new T();
		if (res->LoadFromFile(name))
		{
			mResources.insert(std::make_pair(name, res));
		}
		else
		{
			delete res;
			res = nullptr;
		}
	}

	CHECK(res);

	res->AddRef();
	
Exit0:
	return res;
}
