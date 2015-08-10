#pragma once

class BoxCollider : public SECore::BoxCollider
{
public:
	virtual ~BoxCollider();
	virtual void SetSize(const Vector3& size);
	virtual void SetLocalPose(const Vector3& pos, const Quat& rot);
	virtual bool Init(bool isDynamic);
	virtual void Update(float deltaTime);
	virtual void EnableGravity(bool enable);
	virtual void SetMass(float mass);
public:
	BoxCollider(SceneEntity& owner);
private:
	void UpdateGizmo();
private:
	SceneEntity& mOwner;
	PxMaterial* mMaterial;
	PxRigidActor* mActor;
	Gizmo* mGizmo;
	PxShape* mShape;
private:
	Vector3 mPos;
	Quat mRot;
};
