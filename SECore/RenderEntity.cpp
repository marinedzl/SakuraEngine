#include "stdafx.h"
#include "Material.h"
#include "SceneEntity.h"
#include "RenderEntity.h"

RenderEntity::RenderEntity(SceneEntity& owner)
	: mOwner(owner)
{
	mMaterial = new Material();
}

RenderEntity::~RenderEntity()
{
	SAFE_DELETE(mMaterial);
}

IMesh* RenderEntity::GetMesh()
{
	return nullptr;
}

IMaterial* RenderEntity::GetMaterial()
{
	return mMaterial;
}

void RenderEntity::SetMesh(IMesh* mesh)
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
