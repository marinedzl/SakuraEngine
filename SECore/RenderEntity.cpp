#include "stdafx.h"
#include "SceneEntity.h"
#include "RenderEntity.h"

RenderEntity::RenderEntity(SceneEntity& owner)
	: mOwner(owner)
{
}

RenderEntity::~RenderEntity()
{
}

Mesh* RenderEntity::GetMesh()
{
	return nullptr;
}

Material* RenderEntity::GetMaterial()
{
	return nullptr;
}

void RenderEntity::SetMesh(Mesh* mesh)
{

}

void RenderEntity::SetMaterial(Material* material)
{

}

const Matrix& RenderEntity::GetWorld() const
{
	return mOwner.GetWorld();
}

void RenderEntity::SetWorld(const Matrix& m)
{
	mOwner.SetWorld(m);
}
