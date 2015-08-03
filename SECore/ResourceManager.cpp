#include "stdafx.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshShader.h"
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
	mShaderFactories.insert(std::make_pair(std::string("Default"), new MeshShaderFactory()));
	return true;
}

ITexture* ResourceManager::LoadTexture(const char* filename)
{
	return LoadResource<Texture>(filename);
}

IMesh* ResourceManager::LoadMesh(const char* filename)
{
	return LoadResource<Mesh>(filename);
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
T* ResourceManager::LoadResource(const char* filename)
{
	T* res = nullptr;
	Resources::iterator iter;

	CHECK(filename);

	iter = mResources.find(filename);
	if (iter != mResources.end())
	{
		res = dynamic_cast<T*>(iter->second);
	}
	else
	{
		res = new T();
		if (res->LoadFromFile(filename))
		{
			mResources.insert(std::make_pair(filename, res));
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
