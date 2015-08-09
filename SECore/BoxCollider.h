#pragma once

class BoxCollider : public SECore::BoxCollider
{
public:
	virtual ~BoxCollider();
	virtual void SetSize(const Vector3& size);
	virtual void SetLocalPose(const Vector3& pos, const Quat& rot);
public:
	BoxCollider(SceneEntity& owner);
	bool Init();
private:
	void UpdateGizmo();
private:
	Gizmo* mGizmo;
	SceneEntity& mOwner;
	PxMaterial* mMaterial;
	PxRigidStatic* mRigid;
	PxShape* mShape;
	Vector3 mPos;
	Quat mRot;
};
