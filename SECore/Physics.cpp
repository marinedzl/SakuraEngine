#include "stdafx.h"
#include "Physics.h"

PhysicsCore gPhysicsCore;

class MyPxErrorCallback : public PxErrorCallback
{
public:
	virtual ~MyPxErrorCallback() {}
	virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		log("physx error(%d) : %s\n", code, message);
	}
};

PhysicsCore::PhysicsCore()
	: mFoundation(nullptr)
	, mPhysics(nullptr)
{
}

PhysicsCore::~PhysicsCore()
{
}

bool PhysicsCore::Init()
{
	bool ret = false;

	static MyPxErrorCallback gErrorCallback;
	static PxDefaultAllocator gAllocatorCallback;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocatorCallback, gErrorCallback);
	CHECK(mFoundation);

	bool recordMemoryAllocations = true;
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		PxTolerancesScale(), recordMemoryAllocations);
	CHECK(mPhysics);

	CHECK(PxInitExtensions(*mPhysics));

	ret = true;
Exit0:
	return ret;
}

void PhysicsCore::Release()
{
	if (mPhysics)
		mPhysics->release();
	mPhysics = nullptr;

	if (mFoundation)
		mFoundation->release();
	mFoundation = nullptr;
}
