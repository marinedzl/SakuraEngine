#pragma once

class Collider : public SECore::Collider
{
public:
	virtual ~Collider();
	virtual SECore::RigidBody* GetRigidBody();
	virtual void SetLocalPose(const Vector3& pos, const Quat& rot);
	virtual bool SetPos(const Vector3 & pos);
public:
	Collider(SceneEntity& owner);
	bool Init(bool isDynamic);
	virtual void OnPhysicsUpdateTransform(const Vector3& pos, const Quat& rot);
	virtual void OnInitShape() = 0;
protected:
	Transform mTransform;
	SceneEntity& mOwner;
	RigidBody* mRigidBody;
	PxRigidActor* mActor;
	Gizmo* mGizmo;
	PxShape* mShape;
};
