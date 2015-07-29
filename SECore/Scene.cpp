#include "stdafx.h"
#include "MeshRenderer.h"
#include "SceneEntity.h"
#include "SceneLoader.h"
#include "ConstantBufferManager.h"
#include "Core.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Release()
{
	ClearEntities();
	delete this;
}

Scene::Entity* Scene::CreateEntity()
{
	SceneEntity* entity = new SceneEntity();
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

void Scene::ClearEntities()
{
	DeleteList(mEntities);
}

bool Scene::Init()
{
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

	return true;
}

bool Scene::LoadAdditive(const char * filename)
{
	return SceneLoader::Load(this, filename);
}

void Scene::Draw(IRenderTarget* rt)
{
	mCamera.rtW = rt->GetWidth();
	mCamera.rtH = rt->GetHeight();

	D3D11_VIEWPORT viewport;
	ID3D11DeviceContext* context = gCore.GetContext();
	CHECK(context);

	CBGlobal* cbGlobal = gConstantBufferManager.GetBuffer<CBGlobal>();
	CHECK(cbGlobal);

	mCamera.GetViewport(viewport);
	context->RSSetViewports(1, &viewport);

	mCamera.GetViewProjMatrix(cbGlobal->MATRIX_VP);
	gConstantBufferManager.Commit<CBGlobal>();

	DrawScene();

Exit0:
	;
}

void Scene::DrawScene()
{
	if (!mEntities.empty())
	{
		gMeshRenderer.Begin();

		Entities::iterator iter = mEntities.begin();
		Entities::iterator iterEnd = mEntities.end();
		for (; iter != iterEnd; ++iter)
		{
			Entity* entity = *iter;
			if (!entity)
				continue;

			IRenderer* renderer = entity->GetRenderer();
			if (!renderer)
				continue;

			size_t count = renderer->GetEntityCount();
			for (size_t i = 0; i < count; ++i)
			{
				if (IRenderer::Entity* renderEntity = renderer->GetEntity(i))
				{
					gMeshRenderer.Draw(renderEntity);
				}
			}
		}

		gMeshRenderer.End();
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
}
