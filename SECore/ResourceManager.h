#pragma once

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	void Release();
	bool Init();
	ITexture* LoadTexture(const char* filename);
	IShader* LoadShader(const char* filename);
	IMesh* LoadMesh(const char* filename);
	template<typename T>
	T* LoadResource(const char* filename);
private:
	typedef std::map<std::string, RefObject*> Resources;
private:
	Resources mResources;
};

extern ResourceManager gResourceManager;
