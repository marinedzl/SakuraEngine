#include "stdafx.h"
#include "Physics.h"
#include "Gizmo.h"
#include "Scene.h"
#include "SceneEntity.h"
#include "ShapeMesh.h"
#include "SceneEntity.h"
#include "CharacterController.h"

CharacterController::CharacterController(SceneEntity& owner)
	: mOwner(owner)
	, mMinDst(0)
{
}

CharacterController::~CharacterController()
{
	//if (mController)
		//mController->release();
	mOwner.GetScene().RemoveGizmo(mGizmo);
	SAFE_DELETE(mGizmo);
}

bool CharacterController::Init(PxControllerManager* manager, float height, float radius, const Vector3& offset)
{
	bool ret = false;

	mOffset = offset;
	const Vector3& pos = mOwner.GetTransform().position + mOffset;
	PxCapsuleControllerDesc desc;
	desc.height = height;
	desc.radius = radius;
	desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
	desc.material = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	CHECK(desc.isValid());
	mController = manager->createController(desc);
	CHECK(mController);

	mController->getActor()->userData = &mOwner;

	mGizmo = new Gizmo();
	mGizmo->SetMesh(ShapeMesh::CreateCapsule(height * 0.5f, radius));
	mOwner.GetScene().AddGizmo(mGizmo);

	mGizmo->SetColor(Color(0, 1, 0, 1));

	ret = true;
Exit0:
	return ret;
}

void CharacterController::Update(float deltaTime)
{
	PxExtendedVec3 pv = mController->getPosition();
	Vector3 pos((float)pv.x, (float)pv.y, (float)pv.z);
	pos = pos - mOffset;
	mOwner.GetTransform().position = pos;
	UpdateGizmo();
}

bool CharacterController::Move(const Vector3 & offset, float elapsedTime)
{
	PxControllerCollisionFlags collisionFlags = mController->move(ConvertPxVec3(offset), mMinDst, elapsedTime, PxControllerFilters());
	return collisionFlags;
}

void CharacterController::UpdateGizmo()
{
	mGizmo->GetTransform().position = mOwner.GetTransform().position + mOffset;
	mGizmo->GetTransform().rotation = mOwner.GetTransform().rotation;
}
