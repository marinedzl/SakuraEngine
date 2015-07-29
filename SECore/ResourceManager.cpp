#include "stdafx.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
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
	DeleteMap(mResources);
}

bool ResourceManager::Init()
{
	return true;
}

ITexture* ResourceManager::LoadTexture(const char* filename)
{
	return LoadResource<Texture>(filename);
}

IShader* ResourceManager::LoadShader(const char* filename)
{
	std::string fullpath = filename;
	fullpath += ".shader";
	return LoadResource<Shader>(fullpath.c_str());
}

IMesh* ResourceManager::LoadMesh(const char* filename)
{
	return LoadResource<Mesh>(filename);
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
