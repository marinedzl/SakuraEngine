#pragma once

class BoxCollider : public SECore::BoxCollider
{
public:
	virtual ~BoxCollider();
	virtual void SetSize(const Vector3& size);
public:
	BoxCollider(SceneEntity& owner);
	bool Init();
private:
	SceneEntity& mOwner;
	PxMaterial* mMaterial;
	PxRigidStatic* mRigid;
	Vector3 mSize;
};
