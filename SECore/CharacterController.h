#pragma once

class CharacterController : public SECore::CharacterController
{
public:
	virtual ~CharacterController();
	virtual bool Move(const Vector3 & offset, float elapsedTime);
public:
	CharacterController(SceneEntity& owner);
	bool Init(PxControllerManager* manager, float height, float radius);
private:
	SceneEntity& mOwner;
	PxController* mController;
	float mMinDst;
};
