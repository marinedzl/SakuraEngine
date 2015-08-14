#include "stdafx.h"
#include "AnimationClip.h"
#include "AnimationState.h"

AnimationState::AnimationState()
	: AnimationState(nullptr)
{
}

AnimationState::AnimationState(const AnimationClip* clip)
{
	Reset(clip);
}

AnimationState::~AnimationState()
{
}

void AnimationState::Reset(const AnimationClip* clip)
{
	this->mClip = clip;
	mElapsedTime = 0;
	mClipLength = clip ? clip->GetLength() : 0;
	mWeight = 1.0f;
}
