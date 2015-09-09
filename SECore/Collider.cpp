#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "Gizmo.h"
#include "ShapeMesh.h"
#include "RigidBody.h"
#include "Collider.h"

Collider::Collider(SceneEntity& owner)
	: mOwner(owner)
	, mActor(nullptr)
	, mShape(nullptr)
	, mGizmo(nullptr)
	, mRigidBody(nullptr)
{
}

Collider::~Collider()
{
	if (mActor)
		mActor->release();
	if (mGizmo)
		mOwner.GetScene().RemoveGizmo(mGizmo);
	SAFE_DELETE(mGizmo);
	SAFE_DELETE(mRigidBody);
}

bool Collider::Init(bool isDynamic)
{
	bool ret = false;

	const Transform& transform = mOwner.GetTransform();

	mTransform.position = transform.position;
	mTransform.rotation = transform.rotation;

	PxVec3 pos = ConvertPxVec3(transform.position);
	PxQuat rot = ConvertPxQuat(transform.rotation);

	PxScene* scene = mOwner.GetScene().GetPxScene();
	CHECK(scene);

	if (isDynamic)
	{
		PxRigidDynamic* dyn = gPhysics->createRigidDynamic(PxTransform(pos, rot));
		dyn->setLinearDamping(0.25);
		dyn->setAngularDamping(0.25);
		mActor = dyn;
		mRigidBody = new RigidBody(*dyn);
	}
	else
	{
		mActor = gPhysics->createRigidStatic(PxTransform(pos, rot));
	}

	CHECK(mActor);

	mActor->userData = &mOwner;

	OnInitShape();

	CHECK(mGizmo);
	mOwner.GetScene().AddGizmo(mGizmo);

	mGizmo->SetColor(Color(0, 1, 0, 1));

	scene->addActor(*mActor);

	SetLocalPose(Vector3(), Quat());

	ret = true;
Exit0:
	return ret;
}

SECore::RigidBody* Collider::GetRigidBody()
{
	return mRigidBody;
}

void Collider::SetLocalPose(const Vector3& pos, const Quat& rot)
{
	mShape->setLocalPose(PxTransform(ConvertPxVec3(pos), ConvertPxQuat(rot)));
	mGizmo->GetLocal().position = pos;
	mGizmo->GetLocal().rotation = rot;
}

bool Collider::SetPos(const Vector3 & pos)
{
	PxTransform pt = mActor->getGlobalPose();
	pt.p = ConvertPxVec3(pos);
	mActor->setGlobalPose(pt);
	return true;
}

void Collider::OnPhysicsUpdateTransform(const Vector3& pos, const Quat& rot)
{
	mOwner.GetTransform().position = pos;
	mOwner.GetTransform().rotation = rot;
	mTransform.position = pos;
	mTransform.rotation = rot;
}
