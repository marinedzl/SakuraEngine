#include "stdafx.h"
#include "Renderer.h"
#include "Animation.h"
#include "BoxCollider.h"
#include "SceneEntity.h"

SceneEntity::SceneEntity(Scene& scene)
	: mScene(scene)
	, mRenderer(nullptr)
	, mAnimation(nullptr)
	, mCollider(nullptr)
{
}

SceneEntity::~SceneEntity()
{
	DestroyAnimation();
	DestroyRenderer();
	DestroyCollider();
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

SECore::Collider * SceneEntity::CreateCollider(Collider::Type type)
{
	CHECK(!mCollider);
	switch (type)
	{
	case Collider::eBox:
		mCollider = new BoxCollider(*this);
		break;
	}
Exit0:
	return mCollider;
}

void SceneEntity::Update(float deltaTime)
{
	if (mAnimation)
		mAnimation->Update(deltaTime);
}
