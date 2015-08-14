#pragma once
#include "AnimationState.h"

class AnimationBlender
{
public:
	AnimationBlender();
	virtual ~AnimationBlender();
	void Play(const AnimationClip* clip);
	void CrossFade(const AnimationClip* clip, float fadeLength);
	void Update(float deltaTime);
	const AnimationState* GetCurrentState() const { return mCurrentState.IsValid() ? &mCurrentState : nullptr; }
	const AnimationState* GetNextState() const { return mNextState.IsValid() ? &mNextState : nullptr; }
private:
	float mFadeLength;
	float mFadeElapsedTime;
	AnimationState mCurrentState;
	AnimationState mNextState;
};
