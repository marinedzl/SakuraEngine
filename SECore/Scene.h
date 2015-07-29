#pragma once
#include "Camera.h"

class Scene : public IScene
{
	friend class SceneLoader;
public:
	Scene();
	virtual ~Scene();
public:
	virtual bool Init();
	virtual bool LoadAdditive(const char* filename);
	virtual void Draw(IRenderTarget* rt);
	virtual void Update(float deltaTime);
	virtual ICamera* GetCamera() { return &mCamera; }
public:
	virtual void Release();
	virtual void ClearEntities();
	virtual Entity* CreateEntity();
	virtual void RemoveEntity(Entity* entity);
private:
	void DrawScene();
private:
	typedef std::list<SceneEntity*> Entities;
private:
	Camera mCamera;
	Entities mEntities;
};
