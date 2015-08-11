#include "stdafx.h"
#include "SceneEntity.h"
#include "CharacterController.h"

CharacterController::CharacterController(SceneEntity& owner)
	: mOwner(owner)
	, mMinDst(0)
{
}

CharacterController::~CharacterController()
{
	if (mController)
		mController->release();
}

bool CharacterController::Init(PxControllerManager* manager, float height, float radius)
{
	bool ret = false;
	const Vector3& pos = mOwner.GetTransform().position;
	PxCapsuleControllerDesc desc;
	desc.height = height;
	desc.radius = radius;
	desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
	desc.userData = &mOwner;
	mController = manager->createController(desc);
	CHECK(mController);
	ret = true;
Exit0:
	return ret;
}

bool CharacterController::Move(const Vector3 & offset, float elapsedTime)
{
	PxControllerCollisionFlags collisionFlags = mController->move(ConvertPxVec3(offset), mMinDst, elapsedTime, PxControllerFilters());
	return collisionFlags;
}
