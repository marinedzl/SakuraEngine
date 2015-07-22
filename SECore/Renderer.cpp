#include "stdafx.h"
#include "SceneEntity.h"
#include "RenderEntity.h"
#include "Renderer.h"

RendererImpl::RendererImpl(SceneEntity& owner)
	: mOwner(owner)
{
}

RendererImpl::~RendererImpl()
{
}

void RendererImpl::ClearEntities()
{
	DeleteList(mEntities);
}

Renderer::Entity* RendererImpl::CreateEntity()
{
	RenderEntity* entity = new RenderEntity(mOwner);
	mEntities.push_back(entity);
	return entity;
}

size_t RendererImpl::GetEntityCount()
{
	return mEntities.size();
}

Renderer::Entity* RendererImpl::GetEntity(size_t index)
{
	return mEntities[index];
}
