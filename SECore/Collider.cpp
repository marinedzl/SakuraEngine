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
	mOwner.GetScene().RemoveGizmo(mGizmo);
	SAFE_DELETE(mGizmo);
	SAFE_DELETE(mRigidBody);
}

bool Collider::Init(bool isDynamic)
{
	bool ret = false;

	const Transform& transform = mOwner.GetTransform();

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
	mPos = pos;
	mRot = rot;
	mShape->setLocalPose(PxTransform(ConvertPxVec3(pos), ConvertPxQuat(rot)));
	UpdateGizmo();
}

void Collider::OnPhysicsUpdateTransform(const Vector3& pos, const Quat& rot)
{
	mOwner.GetTransform().position = pos;
	mOwner.GetTransform().rotation = rot;
	UpdateGizmo();
}

void Collider::UpdateGizmo()
{
	mGizmo->GetTransform().position = mOwner.GetTransform().position + mPos;
	mGizmo->GetTransform().rotation = mOwner.GetTransform().rotation;
}
