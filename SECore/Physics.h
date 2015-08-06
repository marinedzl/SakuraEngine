#pragma once

class PhysicsCore
{
public:
	PhysicsCore();
	virtual ~PhysicsCore();
	bool Init();
	void Release();
	PxPhysics* GetPhysics() { return mPhysics; }
private:
	PxFoundation* mFoundation;
	PxPhysics* mPhysics;
};

extern PhysicsCore gPhysicsCore;
#define gPhysics gPhysicsCore.GetPhysics()
