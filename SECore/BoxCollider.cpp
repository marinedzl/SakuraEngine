#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "Gizmo.h"
#include "ShapeMesh.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(SceneEntity& owner)
	: Collider(owner)
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::OnInitShape()
{
	PxMaterial* mtl = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(mtl);

	mShape = mActor->createShape(PxBoxGeometry(mSize.x, mSize.y, mSize.z), *mtl);
	CHECK(mShape);

	mGizmo = new Gizmo();
	mGizmo->SetMesh(ShapeMesh::CreateBox(mSize));
	mOwner.GetScene().AddGizmo(mGizmo);

Exit0:
	;
}
