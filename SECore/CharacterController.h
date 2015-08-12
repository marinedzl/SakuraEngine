#pragma once

class CharacterController : public SECore::CharacterController
{
public:
	virtual ~CharacterController();
	virtual bool Move(const Vector3 & offset, float elapsedTime);
public:
	CharacterController(SceneEntity& owner);
	void Update(float deltaTime);
	void UpdateGizmo();
	bool Init(PxControllerManager* manager, float height, float radius, const Vector3& offset);
private:
	Gizmo* mGizmo;
	SceneEntity& mOwner;
	PxController* mController;
	float mMinDst;
	Vector3 mOffset;
};
