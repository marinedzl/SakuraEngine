#include "stdafx.h"
#include "Mesh.h"
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
	SAFE_RELEASE(mMesh);
}

SECore::Mesh* RenderEntity::GetMesh()
{
	return mMesh;
}

SECore::Material* RenderEntity::GetMaterial()
{
	return mMaterial;
}

void RenderEntity::SetMesh(SECore::Mesh* mesh)
{
	SAFE_RELEASE(mMesh);
	if (Mesh* m = dynamic_cast<Mesh*>(mesh))
	{
		mMesh = m;
		mMesh->AddRef();
	}
}

const Matrix& RenderEntity::GetWorld() const
{
	return mOwner.GetWorld();
}

void RenderEntity::SetWorld(const Matrix& m)
{
	mOwner.SetWorld(m);
}

bool RenderEntity::IsSkinned() const
{
	return mOwner.GetAnimation() != nullptr;
}

bool RenderEntity::GetBoneMatrix(Matrix* dst) const
{
	return mOwner.GetSkinMatrix(dst);
}
