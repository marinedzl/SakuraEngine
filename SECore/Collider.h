#pragma once

class Collider : public SECore::Collider
{
public:
	virtual ~Collider();
	virtual void SetLocalPose(const Vector3& pos, const Quat& rot);
	virtual void OnPhysicsUpdateTransform(const Vector3& pos, const Quat& rot);
	virtual void EnableGravity(bool enable);
	virtual void SetMass(float mass);
	virtual void OnInitShape() = 0;
public:
	Collider(SceneEntity& owner);
	bool Init(bool isDynamic);
private:
	void UpdateGizmo();
protected:
	SceneEntity& mOwner;
	PxRigidActor* mActor;
	Gizmo* mGizmo;
	PxShape* mShape;
private:
	Vector3 mPos;
	Quat mRot;
};
