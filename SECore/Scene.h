#pragma once
#include "Camera.h"

class ShadowRenderTarget;
class Scene : public SECore::Scene
{
	friend class SceneLoader;
	struct Config : public SECore::Scene::Config
	{
		Config() : showGizmo(false), ambientColor(1, 1, 1, 1) {}
		virtual void EnableGizmo(bool enable) { showGizmo = enable; }
		virtual void SetAmbientColor(const Color& color) { ambientColor = color; }
		bool showGizmo;
		Color ambientColor;
	};
public:
	virtual ~Scene();
	virtual bool Init();
	virtual void Release();
	virtual SECore::Core* GetCore();
	virtual void ClearEntities();
	virtual Entity* CreateEntity();
	virtual void RemoveEntity(Entity* entity);
	virtual Entity* FindEntity(const char* name);
	virtual void Draw(const SECore::Camera* camera, SECore::RenderTarget* rt);
	virtual void Update(float deltaTime);
	virtual bool Raycast(const Ray& ray, RaycastHit& hit, float distance);
	virtual bool RaycastBound(const Ray& ray, RaycastHit& hit, float distance);
	virtual SECore::Scene::Config* GetConfig() { return &mConfig; }
	virtual SECore::Light* AddPointLight();
	virtual SECore::Light* AddDirectionalLight();
	virtual void ClearLights();
	virtual SECore::Light* FindLight(const char* name);
	virtual bool Resize(int w, int h);
public:
public:
	Scene();
	PxScene* GetPxScene() { return mPxScene; }
	PxControllerManager* GetCCTMgr() { return mCCTManager; }
	void AddGizmo(Gizmo* gizmo);
	void RemoveGizmo(Gizmo* gizmo);
private:
	void DrawObjects(RenderPass renderPass);
	void DrawShadow(SECore::Light* light);
	void DrawLight();
	void DrawGizmos(const Camera* camera);
	void CommitGlobal();
private:
	typedef std::list<SceneEntity*> Entities;
	typedef std::list<Gizmo*> Gizmos;
	typedef std::list<SECore::Light*> Lights;
	struct CBGlobal
	{
		Matrix MATRIX_VP;
		Matrix INV_VP;
		Vector3 EYE_POS; float nouse;
		Vector2 SCREEN_SIZE; Vector2 nouse2;
		Color AmbientColor;
	};
private:
	Config mConfig;
	PxScene* mPxScene;
	PxDefaultCpuDispatcher* mCpuDispatcher;
	PxControllerManager* mCCTManager;
	Entities mEntities;
	Gizmos mGizmos;
	Lights mLights;

	ID3D11Buffer* mCBGlobal;

	ID3D11PixelShader* mGBufferPS;
	ID3D11PixelShader* mShadowPS;
	ShadowRenderTarget* mShadowRT;
	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11VertexShader* mLightingVS;
	ID3D11Buffer* mLightingMesh;
	ID3D11InputLayout* mInputLayout;

	CBGlobal mSBGloal;

	float mElapsedTime;
};
