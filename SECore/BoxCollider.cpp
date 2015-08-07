#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "Gizmo.h"
#include "ShapeMesh.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(SceneEntity& owner)
	: mOwner(owner)
	, mSize(1, 1, 1)
	, mRigid(nullptr)
	, mShape(nullptr)
	, mGizmo(nullptr)
{
}

BoxCollider::~BoxCollider()
{
	if (mRigid)
		mRigid->release();
	mOwner.GetScene().RemoveGizmo(mGizmo);
	SAFE_DELETE(mGizmo);
}

bool BoxCollider::Init()
{
	bool ret = false;

	const Transform& transform = mOwner.GetTransform();
	Vector3 scaling = transform.scaling;

	PxVec3 pos = ConvertPxVec3(transform.position);
	PxQuat rot = GetPxQuat(transform.rotation);

	PxScene* scene = mOwner.GetScene().GetPxScene();
	CHECK(scene);

	mMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(mMaterial);

	mRigid = gPhysics->createRigidStatic(PxTransform(pos, rot));
	CHECK(mRigid);

	scaling.x = 1 / scaling.x;
	scaling.y = 1 / scaling.y;
	scaling.z = 1 / scaling.z;

	SetSize(Vector3(0.5f, 0.5f, 0.5f));

	scene->addActor(*mRigid);

	ret = true;
Exit0:
	return ret;
}

void BoxCollider::SetSize(const Vector3& size)
{
	CHECK(mRigid);

	mSize = size;

	if (mShape)
	{
		mRigid->detachShape(*mShape);
	}

	mShape = mRigid->createShape(PxBoxGeometry(mSize.x, mSize.y, mSize.z), *mMaterial);
	CHECK(mShape);

	mRigid->attachShape(*mShape);

	if (!mGizmo)
	{
		Transform transform = mOwner.GetTransform();
		transform.scaling = Vector3(1, 1, 1);
		mGizmo = new Gizmo(transform);
		mOwner.GetScene().AddGizmo(mGizmo);
	}

	if (mGizmo)
	{
		mGizmo->SetMesh(ShapeMesh::CreateBox(mSize));
	}
Exit0:
	;
}
