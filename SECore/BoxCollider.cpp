#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(SceneEntity& owner)
	: mOwner(owner)
	, mSize(1, 1, 1)
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

	PxVec3 pos;
	PxQuat rot;
	float radius = 0.5f;

	PxScene* scene = mOwner.GetScene().GetPxScene();
	CHECK(scene);

	mMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(mMaterial);

	mRigid = gPhysics->createRigidStatic(PxTransform(pos, rot));
	CHECK(mRigid);

	SetSize(mSize);

	scene->addActor(*mRigid);

	ret = true;
Exit0:
	return ret;
}

void BoxCollider::SetSize(const Vector3& size)
{
	CHECK(mRigid);

	PxShape* shape = mRigid->createShape(PxBoxGeometry(size.x, size.y, size.z), *mMaterial);
	CHECK(shape);

	mRigid->attachShape(*shape);
Exit0:
	;
}
