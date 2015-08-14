#include "stdafx.h"
#include "AnimationBlender.h"

AnimationBlender::AnimationBlender()
{
}

AnimationBlender::~AnimationBlender()
{
}

void AnimationBlender::Play(const AnimationClip* clip)
{
	mCurrentState.Reset(clip);
	mNextState.Reset(nullptr);
}

void AnimationBlender::CrossFade(const AnimationClip* clip, float fadeLength)
{
	mFadeLength = fadeLength;
	mFadeElapsedTime = 0;
	mNextState.Reset(clip);
}

void AnimationBlender::Update(float deltaTime)
{
	if (!mCurrentState.IsValid())
		return;

	if (mNextState.IsValid())
	{
		mFadeElapsedTime += deltaTime;

		mCurrentState.AddTime(deltaTime);
		mNextState.AddTime(deltaTime);

		if (mFadeElapsedTime >= mFadeLength)
		{
			mCurrentState = mNextState;
			mNextState.Reset(nullptr);
		}
		else
		{
			float lerp = mFadeElapsedTime / mFadeLength;
			mCurrentState.SetWeight(1 - lerp);
			mNextState.SetWeight(lerp);
		}
	}
	else
	{
		float time = fmod(mCurrentState.GetTime() + deltaTime, mCurrentState.GetLength());
		mCurrentState.SetTime(time);
	}
}
