#pragma once

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	void Release();
	bool Init();
	ITexture* LoadTexture(const char* filename);
	IMesh* LoadMesh(const char* filename);
	AnimationClip* LoadClip(const char* filename);
	Skeleton* LoadSkeleton(const char* filename);
	template<typename T>
	T* LoadResource(const char* filename);
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
