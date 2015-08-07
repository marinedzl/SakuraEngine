#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(SceneEntity& owner)
	: mOwner(owner)
	, mSize(1, 1, 1)
	, mRigid(nullptr)
	, mShape(nullptr)
{
}

BoxCollider::~BoxCollider()
{
	if (mRigid)
		mRigid->release();
}

bool BoxCollider::Init()
{
	bool ret = false;

	const Transform& transform = mOwner.GetTransform();

	PxVec3 pos = ConvertPxVec3(transform.position);
	PxQuat rot = GetPxQuat(transform.rotation);

	PxScene* scene = mOwner.GetScene().GetPxScene();
	CHECK(scene);

	mMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(mMaterial);

	mRigid = gPhysics->createRigidStatic(PxTransform(pos, rot));
	CHECK(mRigid);

	SetSize(transform.scaling);

	scene->addActor(*mRigid);

	ret = true;
Exit0:
	return ret;
}

void BoxCollider::SetSize(const Vector3& size)
{
	CHECK(mRigid);

	if (mShape)
	{
		mRigid->detachShape(*mShape);
	}

	mShape = mRigid->createShape(PxBoxGeometry(size.x, size.y, size.z), *mMaterial);
	CHECK(mShape);

	mRigid->attachShape(*mShape);
Exit0:
	;
}
