#include "stdafx.h"
#include "Renderer.h"
#include "Animator.h"
#include "Animation.h"
#include "BoxCollider.h"
#include "CharacterController.h"
#include "Scene.h"
#include "SceneEntity.h"

SceneEntity::SceneEntity(Scene& scene)
	: mScene(scene)
	, mRenderer(nullptr)
	, mAnimation(nullptr)
	, mAnimator(nullptr)
	, mCollider(nullptr)
	, mCCT(nullptr)
{
}

SceneEntity::~SceneEntity()
{
	DestroyAnimator();
	DestroyAnimation();
	DestroyRenderer();
	DestroyCollider();
	DestroyCCT();
}

bool SceneEntity::GetSkinMatrix(Matrix* dst) const
{
	return mAnimator ? mAnimator->GetMatrix(dst) : mAnimation->GetMatrix(dst);
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
	mAnimation = new Animation();
Exit0:
	return mAnimation;
}

void SceneEntity::DestroyAnimator()
{
	SAFE_DELETE(mAnimator);
}

SECore::Animator* SceneEntity::GetAnimator()
{
	return mAnimator;
}

SECore::Animator* SceneEntity::CreateAnimator()
{
	CHECK(!mAnimator);
	mAnimator = new Animator();
Exit0:
	return mAnimator;
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

SECore::CharacterController* SceneEntity::CreateCCT(float height, float radius, const Vector3& offset)
{
	SAFE_DELETE(mCCT);
	CharacterController* cct = new CharacterController(*this);
	CHECK(cct->Init(mScene.GetCCTMgr(), height, radius, offset));
	mCCT = cct;
Exit0:
	return mCCT;
}

void SceneEntity::Update(float deltaTime)
{
	if (mAnimation)
		mAnimation->Update(deltaTime);
	if (mAnimator)
		mAnimator->Update(deltaTime);
	if (mCCT)
		mCCT->Update(deltaTime);
}
