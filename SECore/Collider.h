#pragma once

class Collider : public SECore::Collider
{
public:
	virtual ~Collider();
	virtual SECore::RigidBody* GetRigidBody();
	virtual void SetLocalPose(const Vector3& pos, const Quat& rot);
public:
	Collider(SceneEntity& owner);
	bool Init(bool isDynamic);
	virtual void OnPhysicsUpdateTransform(const Vector3& pos, const Quat& rot);
	virtual void OnInitShape() = 0;
private:
	void UpdateGizmo();
protected:
	SceneEntity& mOwner;
	RigidBody* mRigidBody;
	PxRigidActor* mActor;
	Gizmo* mGizmo;
	PxShape* mShape;
private:
	Vector3 mPos;
	Quat mRot;
};
