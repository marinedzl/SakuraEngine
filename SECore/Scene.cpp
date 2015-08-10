#include "stdafx.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "GizmoRenderer.h"
#include "SceneEntity.h"
#include "SceneLoader.h"
#include "Core.h"
#include "Physics.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Release()
{
	if (mPxScene)
		mPxScene->release();
	if (mCpuDispatcher)
		mCpuDispatcher->release();
	ClearEntities();
	delete this;
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

	ret = true;
Exit0:
	return ret;
}

bool Scene::LoadAdditive(const char * filename)
{
	return SceneLoader::Load(this, filename);
}

void Scene::Draw(SECore::RenderTarget* rt)
{
	mCamera.rtW = rt->GetWidth();
	mCamera.rtH = rt->GetHeight();

	D3D11_VIEWPORT viewport;
	ID3D11DeviceContext* context = gCore.GetContext();
	CHECK(context);

	mCamera.GetViewport(viewport);
	context->RSSetViewports(1, &viewport);

	DrawScene();
	DrawGizmos();

Exit0:
	;
}

void Scene::DrawScene()
{
	if (!mEntities.empty())
	{
		gMeshRenderer.Begin(mCamera);

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
					gMeshRenderer.Draw(renderEntity);
				}
			}
		}

		gMeshRenderer.End();
	}
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

void Scene::Update(float deltaTime)
{
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
				if (Collider* collider = entity->GetCollider())
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

void Scene::AddGizmo(Gizmo* gizmo)
{
	mGizmos.push_back(gizmo);
}

void Scene::RemoveGizmo(Gizmo* gizmo)
{
	mGizmos.remove(gizmo);
}
