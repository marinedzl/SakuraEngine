#include "stdafx.h"
#include "Renderer.h"
#include "Animation.h"
#include "SceneEntity.h"

SceneEntity::SceneEntity()
	: mRenderer(nullptr)
{
}

SceneEntity::~SceneEntity()
{
	DestroyAnimation();
	DestroyRenderer();
}

void SceneEntity::SetWorld(const Matrix& m)
{
	mWorld = m;
}

const Matrix& SceneEntity::GetWorld() const
{
	return mWorld;
}

void SceneEntity::DestroyRenderer()
{
	SAFE_DELETE(mRenderer);
}

Renderer* SceneEntity::GetRenderer()
{
	return mRenderer;
}

Renderer* SceneEntity::CreateRenderer()
{
	CHECK(!mRenderer);
	mRenderer = new RendererImpl(*this);
Exit0:
	return mRenderer;
}

void SceneEntity::DestroyAnimation()
{
	SAFE_DELETE(mAnimation);
}

Animation* SceneEntity::GetAnimation()
{
	return mAnimation;
}

Animation* SceneEntity::CreateAnimation()
{
	CHECK(!mAnimation);
	mAnimation = new AnimationImpl(*this);
Exit0:
	return mAnimation;
}

void SceneEntity::Update(float deltaTime)
{
	if (mAnimation)
	{
		mAnimation->Update(deltaTime);
	}
}
