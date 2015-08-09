#include "stdafx.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "Physics.h"
#include "Gizmo.h"
#include "ShapeMesh.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(SceneEntity& owner)
	: mOwner(owner)
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

	PxVec3 pos = ConvertPxVec3(transform.position);
	PxQuat rot = ConvertPxQuat(transform.rotation);

	PxScene* scene = mOwner.GetScene().GetPxScene();
	CHECK(scene);

	mMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(mMaterial);

	mRigid = gPhysics->createRigidStatic(PxTransform(pos, rot));
	CHECK(mRigid);

	SetSize(Vector3(0.5f, 0.5f, 0.5f));

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
		mShape = nullptr;
	}

	mShape = mRigid->createShape(PxBoxGeometry(size.x, size.y, size.z), *mMaterial);
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

void BoxCollider::UpdateGizmo()
{
	mGizmo->GetTransform().position = mOwner.GetTransform().position + mPos;
	mGizmo->GetTransform().rotation = mOwner.GetTransform().rotation;
}
