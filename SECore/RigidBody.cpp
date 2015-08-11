#include "stdafx.h"
#include "RigidBody.h"

RigidBody::RigidBody(PxRigidBody& actor)
	: mActor(actor)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::SetMass(float mass)
{
	mActor.setMass(mass);
}

void RigidBody::EnableGravity(bool enable)
{
	mActor.setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !enable);
}

void RigidBody::SetLinearVelocity(const Vector3 & linVel)
{
	mActor.setLinearVelocity(ConvertPxVec3(linVel));
}

void RigidBody::SetAngularVelocity(const Vector3 & angVel)
{
	mActor.setAngularVelocity(ConvertPxVec3(angVel));
}
