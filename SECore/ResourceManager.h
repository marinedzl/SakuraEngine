#pragma once

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	void Release();
	bool Init();
	Texture* LoadTexture(const char* name);
	Mesh* LoadMesh(const char* name);
	AnimationClip* LoadClip(const char* name);
	Skeleton* LoadSkeleton(const char* name);
	template<typename T>
	T* LoadResource(const char* name);
	bool AddShaderFactory(const char* name, ShaderFactory* factory);
	ShaderFactory* FindShaderFactory(const char* name);
	Shader* CreateShader(const char* name);
private:
	typedef std::map<std::string, RefObject*> Resources;
	typedef std::map<std::string, ShaderFactory*> ShaderFactories;
private:
	Resources mResources;
	ShaderFactories mShaderFactories;
};

extern ResourceManager gResourceManager;
