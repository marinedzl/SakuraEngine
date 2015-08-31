#include "stdafx.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "GizmoRenderer.h"
#include "SceneEntity.h"
#include "RenderEntity.h"
#include "Core.h"
#include "Physics.h"
#include "Collider.h"
#include "RenderTexture.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "RenderStateManager.h"
#include "Texture.h"
#include "Scene.h"

Scene::Scene()
	: mPxScene(nullptr)
	, mCCTManager(nullptr)
	, mCpuDispatcher(nullptr)
	, mGBufferRT(nullptr)
	, mLightingRT(nullptr)
	, mElapsedTime(0)
{
}

Scene::~Scene()
{
}

void Scene::Release()
{
	if (mCCTManager)
		mCCTManager->release();
	if (mPxScene)
		mPxScene->release();
	if (mCpuDispatcher)
		mCpuDispatcher->release();

	ClearEntities();
	ClearLights();

	SAFE_RELEASE(mCBGlobal);

	SAFE_RELEASE(mGBufferRT);
	SAFE_RELEASE(mShadowRT);
	SAFE_RELEASE(mLightingRT);
	SAFE_RELEASE(mGBufferPS);
	SAFE_RELEASE(mShadowPS);

	SAFE_RELEASE(mLightingVS);
	SAFE_RELEASE(mLightingMesh);
	SAFE_RELEASE(mInputLayout);

	delete this;
}

SECore::Core* Scene::GetCore()
{
	return &gCore;
}

Scene::Entity* Scene::CreateEntity()
{
	SceneEntity* entity = new SceneEntity(*this);
	mEntities.push_back(entity);
	return entity;
}

void Scene::RemoveEntity(Entity* entity)
{
	SceneEntity* sceneEntity = dynamic_cast<SceneEntity*>(entity);
	if (sceneEntity)
	{
		mEntities.remove(sceneEntity);
	}
}

Scene::Entity * Scene::FindEntity(const char * name)
{
	Entities::iterator iter = mEntities.begin();
	Entities::iterator iterEnd = mEntities.end();
	for (; iter != iterEnd; ++iter)
	{
		if (SceneEntity* entity = *iter)
		{
			if (strcmp(entity->GetName(), name) == 0)
			{
				return entity;
			}
		}
	}
	return nullptr;
}

void Scene::ClearEntities()
{
	DeleteList(mEntities);
}

void Scene::ClearLights()
{
	DeleteList(mLights);
}

SECore::Light * Scene::FindLight(const char * name)
{
	Lights::iterator iter = mLights.begin();
	Lights::iterator iterEnd = mLights.end();
	for (; iter != iterEnd; ++iter)
	{
		if (SECore::Light* light = *iter)
		{
			if (strcmp(light->GetName(), name) == 0)
			{
				return light;
			}
		}
	}
	return nullptr;
}

bool Scene::Init()
{
	bool ret = false;

	mCamera.projectType = Camera::Perspective;
	mCamera.eye = Vector3(0, 0, -10);
	mCamera.lookat = Vector3(0, 0, 0);
	mCamera.up = Vector3(0, 1, 0);
	mCamera.fov = XM_PI / 4;
	mCamera.znear = 0.1f;
	mCamera.zfar = 1000.0f;
	mCamera.viewH = 1;
	mCamera.viewW = 1;
	mCamera.viewportX = 0;
	mCamera.viewportY = 0;
	mCamera.viewportW = 1;
	mCamera.viewportH = 1;

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	buffer file;
	CHECK(LoadBinaryFile(file, "GBufferPS.cso"));
	CHECK(SUCCEEDED(gCore.GetDevice()->CreatePixelShader(file.ptr(), file.size(), nullptr, &mGBufferPS)));
	CHECK(LoadBinaryFile(file, "ShadowPS.cso"));
	CHECK(SUCCEEDED(gCore.GetDevice()->CreatePixelShader(file.ptr(), file.size(), nullptr, &mShadowPS)));

	if (!sceneDesc.cpuDispatcher)
	{
		mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		CHECK(mCpuDispatcher);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	mPxScene = gPhysics->createScene(sceneDesc);
	CHECK(mPxScene);

	mCCTManager = PxCreateControllerManager(*mPxScene);
	CHECK(mCCTManager);

	// CreateConstantBuffer
	{
		D3D11_BUFFER_DESC buffDesc;
		ZeroMemory(&buffDesc, sizeof(buffDesc));
		buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDesc.Usage = D3D11_USAGE_DYNAMIC;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		buffDesc.ByteWidth = sizeof(CBGlobal);
		CHECK(SUCCEEDED(gCore.GetDevice()->CreateBuffer(&buffDesc, nullptr, &mCBGlobal)));
	}

	// CreateDepthStencilState
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = FALSE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;

		desc.StencilEnable = FALSE;
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;

		mDepthStencilState = gRenderStateManager.GetRenderState<ID3D11DepthStencilState>(desc);
	}

	// CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		D3D11_RENDER_TARGET_BLEND_DESC& target = desc.RenderTarget[0];
		target.BlendEnable = TRUE;
		target.SrcBlend = D3D11_BLEND_ONE;
		target.DestBlend = D3D11_BLEND_ONE;
		target.BlendOp = D3D11_BLEND_OP_ADD;
		target.SrcBlendAlpha = D3D11_BLEND_ONE;
		target.DestBlendAlpha = D3D11_BLEND_ONE;
		target.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		target.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		mBlendState = gRenderStateManager.GetRenderState<ID3D11BlendState>(desc);
	}

	{
		const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		buffer file;
		CHECK(LoadBinaryFile(file, "deferred_lighting_vs.cso"));
		CHECK(SUCCEEDED(gCore.GetDevice()->CreateVertexShader(file.ptr(), file.size(), nullptr, &mLightingVS)));
		CHECK(SUCCEEDED(gCore.GetDevice()->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), file.ptr(), file.size(), &mInputLayout)));
	}

	{
		const float hl = 1.0f;

		Vector3 verticesData[] =
		{
			Vector3(-hl, hl, 0),
			Vector3(hl, hl, 0),
			Vector3(-hl, -hl, 0),
			Vector3(hl, -hl, 0),
		};

		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA dataDesc;

		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		ZeroMemory(&dataDesc, sizeof(dataDesc));

		bufferDesc.ByteWidth = sizeof(Vector3) * _countof(verticesData);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		dataDesc.pSysMem = verticesData;
		CHECK(SUCCEEDED(gCore.GetDevice()->CreateBuffer(&bufferDesc, &dataDesc, &mLightingMesh)));
	}

	ret = true;
Exit0:
	return ret;
}

void Scene::Update(float deltaTime)
{
	mElapsedTime += deltaTime;

	if (!mEntities.empty())
	{
		Entities::iterator iter = mEntities.begin();
		Entities::iterator iterEnd = mEntities.end();
		for (; iter != iterEnd; ++iter)
		{
			if (Entity* entity = *iter)
			{
				entity->Update(deltaTime);
			}
		}
	}

	if (mPxScene)
	{
		mPxScene->simulate(deltaTime);
		if (mPxScene->fetchResults(true))
		{
			// retrieve array of actors that moved
			PxU32 nbActiveTransforms;
			const PxActiveTransform* activeTransforms = mPxScene->getActiveTransforms(nbActiveTransforms);

			// update each render object with the new transform
			for (PxU32 i = 0; i < nbActiveTransforms; ++i)
			{
				const PxActiveTransform& at = activeTransforms[i];
				SceneEntity* entity = (SceneEntity*)activeTransforms[i].userData;
				if (Collider* collider = entity->GetComponent<Collider>())
				{
					collider->OnPhysicsUpdateTransform(ConvertPxVec3(at.actor2World.p), ConvertPxQuat(at.actor2World.q));
				}
			}
		}
	}
}

bool Scene::Raycast(const Ray& ray, RaycastHit& hit, float distance)
{
	PxVec3 origin = ConvertPxVec3(ray.origin);
	PxVec3 unitDir = ConvertPxVec3(ray.direction);
	PxRaycastBuffer pxhit;

	if (mPxScene->raycast(origin, unitDir, distance, pxhit))
	{
		hit.point = ConvertPxVec3(pxhit.block.position);
		hit.normal = ConvertPxVec3(pxhit.block.normal);
		hit.distance = pxhit.block.distance;
		hit.entity = (SceneEntity*)pxhit.block.actor->userData;
		return true;
	}

	return false;
}

SECore::Light * Scene::AddPointLight()
{
	PointLight* light = new PointLight();
	mLights.push_back(light);
	return light;
}

SECore::Light * Scene::AddDirectionalLight()
{
	DirectionalLight* light = new DirectionalLight();
	mLights.push_back(light);
	return light;
}

void Scene::AddGizmo(Gizmo* gizmo)
{
	mGizmos.push_back(gizmo);
}

void Scene::RemoveGizmo(Gizmo* gizmo)
{
	mGizmos.remove(gizmo);
}

void Scene::Draw(SECore::RenderTarget* rt)
{
	mCamera.rtW = rt->GetWidth();
	mCamera.rtH = rt->GetHeight();

	D3D11_VIEWPORT viewport;
	ID3D11DeviceContext* context = gCore.GetContext();
	CHECK(context);

	if (!mGBufferRT)
	{
		mGBufferRT = new RenderTexture();
		mGBufferRT->Create((int)rt->GetWidth(), (int)rt->GetHeight());
	}

	if (!mShadowRT)
	{
		mShadowRT = new RenderTexture();
		mShadowRT->Create(ShadowMapSize, ShadowMapSize);
	}

	if (!mLightingRT)
	{
		mLightingRT = new RenderTexture();
		mLightingRT->Create((int)rt->GetWidth(), (int)rt->GetHeight());
	}

	context->VSSetConstantBuffers(0, 1, &mCBGlobal);
	context->PSSetConstantBuffers(0, 1, &mCBGlobal);

	context->PSSetShader(mShadowPS, nullptr, 0);
	for (std::list<SECore::Light*>::iterator iter = mLights.begin(); iter != mLights.end(); ++iter)
	{
		SECore::Light* light = *iter;
		if (light && light->IsEnable())
		{
			DrawShadow(light);
		}
	}

	mCamera.GetViewport(viewport);
	context->RSSetViewports(1, &viewport);
	mCamera.GetViewProjMatrix(mSBGloal.MATRIX_VP);
	{
		XMMATRIX invVP = XMLoadFloat4x4((XMFLOAT4X4*)&mSBGloal.MATRIX_VP);
		invVP = XMMatrixInverse(nullptr, invVP);
		XMStoreFloat4x4((XMFLOAT4X4*)&mSBGloal.INV_VP, invVP);
	}
	mSBGloal.EYE_POS = mCamera.eye;
	mSBGloal.SCREEN_SIZE.x = mCamera.rtW;
	mSBGloal.SCREEN_SIZE.y = mCamera.rtH;
	mSBGloal.AmbientColor = mConfig.ambientColor;

	CommitGlobal();
	context->PSSetShader(mGBufferPS, nullptr, 0);
	mGBufferRT->Begin();
	mGBufferRT->Clear(Color(0, 0, 0, 0));
	DrawObjects(eGBufferRenderPass);
	mGBufferRT->End();

	mLightingRT->Begin();
	mLightingRT->Clear(Color(0, 0, 0, 1));
	DrawLight();
	mLightingRT->End();

	rt->Begin();
	rt->Clear(Color(0.278f, 0.278f, 0.278f, 1));
	mLightingRT->SetSlot(3);
	DrawObjects(eNormalRenderPass);
	mLightingRT->SetSlot(3, true);
	rt->End();

	if (mConfig.showGizmo)
		DrawGizmos();

	if (mConfig.captureBuffer)
	{
		if (mElapsedTime > 1)
		{
			static bool bSaved = false;
			if (!bSaved)
			{
				bSaved = true;
				mGBufferRT->CaptureToFile("GBuffer.bmp");
				mLightingRT->CaptureToFile("Lighting.bmp");
				mShadowRT->CaptureToFile("ShadowMap.bmp");
			}
		}
	}

Exit0:
	;
}

void Scene::DrawLight()
{
	ID3D11DeviceContext* context = gCore.GetContext();

	UINT stride = sizeof(Vector3);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 1, &mLightingMesh, &stride, &offset);
	context->VSSetShader(mLightingVS, nullptr, 0);
	context->IASetInputLayout(mInputLayout);

	float factor[4] = { 0 };
	context->OMSetBlendState(mBlendState, factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(mDepthStencilState, 1);
	context->RSSetState(gRenderStateManager.GetRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID));

	mGBufferRT->SetSlot(0);
	mShadowRT->SetSlot(1);

	for (std::list<SECore::Light*>::iterator iter = mLights.begin(); iter != mLights.end(); ++iter)
	{
		SECore::Light* light = *iter;
		if (light && light->IsEnable())
		{
			light->Setup();
			context->Draw(4, 0);
		}
	}

	mGBufferRT->SetSlot(0, true);
	mShadowRT->SetSlot(1, true);
}

void Scene::DrawObjects(RenderPass renderPass)
{
	if (!mEntities.empty())
	{
		gMeshRenderer.Begin();

		Entities::iterator iter = mEntities.begin();
		Entities::iterator iterEnd = mEntities.end();
		for (; iter != iterEnd; ++iter)
		{
			SceneEntity* entity = *iter;
			if (!entity)
				continue;

			Renderer* renderer = entity->GetComponent<Renderer>();
			if (!renderer)
				continue;

			size_t count = renderer->GetEntityCount();
			for (size_t i = 0; i < count; ++i)
			{
				if (RenderEntity* renderEntity = renderer->GetEntityInternal(i))
				{
					if (renderPass != eShadowRenderPass || renderEntity->IsCastShadow())
					{
						gMeshRenderer.Draw(renderEntity, renderPass);
					}
				}
			}
		}

		gMeshRenderer.End();
	}
}

void Scene::DrawShadow(SECore::Light * light)
{
	mShadowRT->Begin();
	mShadowRT->Clear(Color(1, 1, 1, 1));

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = ShadowMapSize;
	viewport.Height = ShadowMapSize;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
	gCore.GetContext()->RSSetViewports(1, &viewport);
	light->GetVP(mSBGloal.MATRIX_VP);
	CommitGlobal();

	DrawObjects(eShadowRenderPass);

	mShadowRT->End();
}

void Scene::DrawGizmos()
{
	if (!mGizmos.empty())
	{
		gGizmosRenderer.Begin();

		Gizmos::iterator iter = mGizmos.begin();
		Gizmos::iterator iterEnd = mGizmos.end();
		for (; iter != iterEnd; ++iter)
		{
			gGizmosRenderer.Draw(mCamera, *iter);
		}

		gGizmosRenderer.End();
	}
}

void Scene::CommitGlobal()
{
	ID3D11DeviceContext* context = gCore.GetContext();
	D3D11_MAPPED_SUBRESOURCE mr;
	if (SUCCEEDED(context->Map(mCBGlobal, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
	{
		memcpy(mr.pData, &mSBGloal, sizeof(CBGlobal));
		context->Unmap(mCBGlobal, 0);
	}
}
