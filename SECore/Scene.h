#pragma once
#include "Camera.h"

class Scene : public SECore::Scene
{
	friend class SceneLoader;
	struct Config : public SECore::Scene::Config
	{
		Config() : showGizmo(false), AmbientColor(1, 1, 1, 1) {}
		virtual void EnableGizmo(bool enable) { showGizmo = enable; }
		virtual void SetAmbientColor(const Color& color) { AmbientColor = color; }
		bool showGizmo;
		Color AmbientColor;
	};
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
	virtual SECore::Scene::Config* GetConfig() { return &mConfig; }
	virtual SECore::Light* AddPointLight();
	virtual SECore::Light* AddDirectionalLight();
	virtual void ClearLights();
	virtual SECore::Light* FindLight(const char* name);
public:
	Scene();
	PxScene* GetPxScene() { return mPxScene; }
	PxControllerManager* GetCCTMgr() { return mCCTManager; }
	void AddGizmo(Gizmo* gizmo);
	void RemoveGizmo(Gizmo* gizmo);
private:
	void DrawObjects(bool drawingGBuffer);
	void DrawLight();
	void DrawGizmos();
private:
	typedef std::list<SceneEntity*> Entities;
	typedef std::list<Gizmo*> Gizmos;
	typedef std::list<SECore::Light*> Lights;
private:
	Config mConfig;
	PxScene* mPxScene;
	PxDefaultCpuDispatcher* mCpuDispatcher;
	PxControllerManager* mCCTManager;
	Camera mCamera;
	Entities mEntities;
	Gizmos mGizmos;
	Lights mLights;

	ID3D11Buffer* mCBGlobal;

	ID3D11PixelShader* mGBufferPS;
	RenderTexture* mGBufferRT;
	RenderTexture* mLightingRT;
	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11VertexShader* mLightingVS;
	ID3D11Buffer* mLightingMesh;
	ID3D11InputLayout* mInputLayout;

	float mElapsedTime;
};
