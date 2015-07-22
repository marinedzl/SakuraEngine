#include "stdafx.h"
#include "SceneEntity.h"
#include "Animation.h"

AnimationImpl::AnimationImpl(SceneEntity& owner)
	: mOwner(owner)
{
}

AnimationImpl::~AnimationImpl()
{
}

void AnimationImpl::Play(const char* clipname)
{

}

bool AnimationImpl::GetMatrix(Matrix* dst, size_t count) const
{
	return false;
}

void AnimationImpl::Update(float deltaTime)
{

}
