#include "stdafx.h"
#include "SceneEntity.h"
#include "Animation.h"

Animation::Animation(SceneEntity& owner)
	: mOwner(owner)
{
}

Animation::~Animation()
{
}

void Animation::Play(const char* clipname)
{

}

bool Animation::GetMatrix(Matrix* dst, size_t count) const
{
	return false;
}

void Animation::Update(float deltaTime)
{

}
