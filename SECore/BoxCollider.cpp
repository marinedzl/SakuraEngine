#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "Gizmo.h"
#include "ShapeMesh.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(SceneEntity& owner)
	: mOwner(owner)
	, mActor(nullptr)
	, mShape(nullptr)
	, mGizmo(nullptr)
{
}

BoxCollider::~BoxCollider()
{
	if (mActor)
		mActor->release();
	mOwner.GetScene().RemoveGizmo(mGizmo);
	SAFE_DELETE(mGizmo);
}

bool BoxCollider::Init(bool isDynamic)
{
	bool ret = false;

	const Transform& transform = mOwner.GetTransform();

	PxVec3 pos = ConvertPxVec3(transform.position);
	PxQuat rot = ConvertPxQuat(transform.rotation);

	PxScene* scene = mOwner.GetScene().GetPxScene();
	CHECK(scene);

	mMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(mMaterial);

	if (isDynamic)
	{
		PxRigidDynamic* dyn = gPhysics->createRigidDynamic(PxTransform(pos, rot));
		dyn->setLinearDamping(0.25);
		dyn->setAngularDamping(0.25);
		mActor = dyn;
	}
	else
	{
		mActor = gPhysics->createRigidStatic(PxTransform(pos, rot));
	}

	CHECK(mActor);

	mActor->userData = &mOwner;

	SetSize(Vector3(0.5f, 0.5f, 0.5f));

	scene->addActor(*mActor);

	SetLocalPose(Vector3(), Quat());

	ret = true;
Exit0:
	return ret;
}

void BoxCollider::SetSize(const Vector3& size)
{
	CHECK(mActor);

	if (mShape)
	{
		mActor->detachShape(*mShape);
		mShape = nullptr;
	}

	mShape = mActor->createShape(PxBoxGeometry(size.x, size.y, size.z), *mMaterial);
	CHECK(mShape);

	if (!mGizmo)
	{
		mGizmo = new Gizmo();
		mOwner.GetScene().AddGizmo(mGizmo);
	}

	if (mGizmo)
	{
		mGizmo->SetMesh(ShapeMesh::CreateBox(size));
	}
Exit0:
	;
}

void BoxCollider::SetLocalPose(const Vector3& pos, const Quat& rot)
{
	mPos = pos;
	mRot = rot;
	mShape->setLocalPose(PxTransform(ConvertPxVec3(pos), ConvertPxQuat(rot)));
	UpdateGizmo();
}

void BoxCollider::OnPhysicsUpdateTransform(const Vector3& pos, const Quat& rot)
{
	mOwner.GetTransform().position = pos;
	mOwner.GetTransform().rotation = rot;
	UpdateGizmo();
}

void BoxCollider::EnableGravity(bool enable)
{
	if (mActor)
	{
		mActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !enable);
	}
}

void BoxCollider::SetMass(float mass)
{
	if (mActor->isRigidDynamic())
	{
		((PxRigidDynamic*)mActor)->setMass(mass);
	}
}

void BoxCollider::UpdateGizmo()
{
	mGizmo->GetTransform().position = mOwner.GetTransform().position + mPos;
	mGizmo->GetTransform().rotation = mOwner.GetTransform().rotation;
}
