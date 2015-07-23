#include "stdafx.h"
#include "SceneEntity.h"
#include "RenderEntity.h"
#include "Renderer.h"

Renderer::Renderer(SceneEntity& owner)
	: mOwner(owner)
{
}

Renderer::~Renderer()
{
}

void Renderer::ClearEntities()
{
	DeleteList(mEntities);
}

Renderer::Entity* Renderer::CreateEntity()
{
	RenderEntity* entity = new RenderEntity(mOwner);
	mEntities.push_back(entity);
	return entity;
}

size_t Renderer::GetEntityCount()
{
	return mEntities.size();
}

Renderer::Entity* Renderer::GetEntity(size_t index)
{
	return mEntities[index];
}
