#include "stdafx.h"
#include "MeshRenderer.h"
#include "SceneEntity.h"
#include "SceneLoader.h"
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

bool Scene::LoadAdditive(const char * filename)
{
	return SceneLoader::Load(this, filename);
}

void Scene::Draw()
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
