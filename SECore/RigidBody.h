#pragma once

class RigidBody : public SECore::RigidBody
{
public:
	virtual ~RigidBody();
	virtual void SetMass(float mass);
	virtual void EnableGravity(bool enable);
	virtual void SetLinearVelocity(const Vector3& linVel);
	virtual void SetAngularVelocity(const Vector3& angVel);
public:
	RigidBody(PxRigidBody& actor);
protected:
	PxRigidBody& mActor;
};
