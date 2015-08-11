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
	PxControllerManager* GetCCTMgr() { return mCCTManager; }
	void AddGizmo(Gizmo* gizmo);
	void RemoveGizmo(Gizmo* gizmo);
private:
	void DrawScene();
	void DrawGizmos();
private:
	typedef std::list<SceneEntity*> Entities;
	typedef std::list<Gizmo*> Gizmos;
private:
	PxScene* mPxScene;
	PxDefaultCpuDispatcher* mCpuDispatcher;
	PxControllerManager* mCCTManager;
	Camera mCamera;
	Entities mEntities;
	Gizmos mGizmos;
};
