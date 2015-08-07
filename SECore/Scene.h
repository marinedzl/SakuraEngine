#pragma once
#include "Camera.h"

class Scene : public SECore::Scene
{
	friend class SceneLoader;
public:
	virtual ~Scene();
	virtual bool Init();
	virtual void Release();
	virtual void ClearEntities();
	virtual Entity* CreateEntity();
	virtual void RemoveEntity(Entity* entity);
	virtual Entity* FindEntity(const char* name);
	virtual bool LoadAdditive(const char* filename);
	virtual void Draw(SECore::RenderTarget* rt);
	virtual void Update(float deltaTime);
	virtual SECore::Camera* GetCamera() { return &mCamera; }
	virtual bool Raycast(const Ray& ray, RaycastHit& hit, float distance);
public:
	Scene();
	PxScene* GetPxScene() { return mPxScene; }
private:
	void DrawScene();
private:
	typedef std::list<SceneEntity*> Entities;
private:
	PxScene* mPxScene;
	Camera mCamera;
	Entities mEntities;
};
