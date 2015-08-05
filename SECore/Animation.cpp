#include "stdafx.h"
#include "Skeleton.h"
#include "SceneEntity.h"
#include "AnimationClip.h"
#include "Animation.h"

Animation::Animation(SceneEntity& owner)
	: mOwner(owner)
{
}

Animation::~Animation()
{
	SAFE_RELEASE(mSkeleton);
	ReleaseMap(mClips);
}

void Animation::SetSkeleton(Skeleton * skeleton)
{
	SAFE_RELEASE(mSkeleton);
	mSkeleton = skeleton;
	if (mSkeleton)
	{
		mSkeleton->AddRef();
	}
}

void Animation::Play(const char* clipname)
{

}

bool Animation::GetMatrix(Matrix* dst, size_t count) const
{
	return false;
}

const AnimationClip * Animation::GetClip(const char * name) const
{
	Clips::const_iterator iter = mClips.find(name);
	return iter == mClips.end() ? nullptr : iter->second;
}

void Animation::Update(float deltaTime)
{

}

bool Animation::AddClip(const char* name, AnimationClip* clip)
{
	clip->AddRef();
	mClips.insert(std::make_pair(name, clip));
	return true;
}
