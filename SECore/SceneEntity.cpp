#include "stdafx.h"
#include "Renderer.h"
#include "Animation.h"
#include "BoxCollider.h"
#include "CharacterController.h"
#include "Scene.h"
#include "SceneEntity.h"

SceneEntity::SceneEntity(Scene& scene)
	: mScene(scene)
	, mRenderer(nullptr)
	, mAnimation(nullptr)
	, mCollider(nullptr)
	, mCCT(nullptr)
{
}

SceneEntity::~SceneEntity()
{
	DestroyAnimation();
	DestroyRenderer();
	DestroyCollider();
	DestroyCCT();
}

bool SceneEntity::GetSkinMatrix(Matrix* dst) const
{
	return mAnimation->GetMatrix(dst);
}

void SceneEntity::DestroyRenderer()
{
	SAFE_DELETE(mRenderer);
}

SECore::Renderer* SceneEntity::GetRenderer()
{
	return mRenderer;
}

SECore::Renderer* SceneEntity::CreateRenderer()
{
	CHECK(!mRenderer);
	mRenderer = new Renderer(*this);
Exit0:
	return mRenderer;
}

void SceneEntity::DestroyAnimation()
{
	SAFE_DELETE(mAnimation);
}

SECore::Animation* SceneEntity::GetAnimation()
{
	return mAnimation;
}

SECore::Animation* SceneEntity::CreateAnimation()
{
	CHECK(!mAnimation);
	mAnimation = new Animation(*this);
Exit0:
	return mAnimation;
}

void SceneEntity::DestroyCollider()
{
	delete mCollider;
}

SECore::Collider * SceneEntity::GetCollider()
{
	return mCollider;
}

SECore::Collider* SceneEntity::CreateBoxCollider(bool isDynamic, const Vector3& size)
{
	SAFE_DELETE(mCollider);
	BoxCollider* collider = new BoxCollider(*this);
	collider->SetSize(size);
	CHECK(collider->Init(isDynamic));
	mCollider = collider;
Exit0:
	return mCollider;
}

void SceneEntity::DestroyCCT()
{
	delete mCCT;
}

SECore::CharacterController* SceneEntity::GetCCT()
{
	return mCCT;
}

SECore::CharacterController* SceneEntity::CreateCCT(float height, float radius)
{
	SAFE_DELETE(mCCT);
	CharacterController* cct = new CharacterController(*this);
	CHECK(cct->Init(mScene.GetCCTMgr(), height, radius));
	mCCT = cct;
Exit0:
	return mCCT;
}

void SceneEntity::Update(float deltaTime)
{
	if (mAnimation)
		mAnimation->Update(deltaTime);
}
