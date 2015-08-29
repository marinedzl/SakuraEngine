#pragma once

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	void Release();
	bool Init();
	SECore::Texture* LoadTexture(const char* name);
	SECore::Mesh* LoadMesh(const char* name);
	SECore::AnimationClip* LoadClip(const char* name);
	SECore::Skeleton* LoadSkeleton(const char* name);
	SECore::Shader* CreateShader(const char* name);
	template<typename T>
	T* LoadResource(const char* name);
	bool AddShaderFactory(const char* name, ShaderFactory* factory);
	ShaderFactory* FindShaderFactory(const char* name);
private:
	typedef std::map<std::string, RefObject*> Resources;
	typedef std::map<std::string, ShaderFactory*> ShaderFactories;
private:
	Resources* mResources;
	ShaderFactories* mShaderFactories;
};

extern ResourceManager gResourceManager;
