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

IRenderer* SceneEntity::GetRenderer()
{
	return mRenderer;
}

IRenderer* SceneEntity::CreateRenderer()
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

IAnimation* SceneEntity::GetAnimation()
{
	return mAnimation;
}

IAnimation* SceneEntity::CreateAnimation()
{
	CHECK(!mAnimation);
	mAnimation = new Animation(*this);
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
