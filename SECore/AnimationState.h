#pragma once

class AnimationState
{
public:
	AnimationState();
	AnimationState(const AnimationClip* clip);
	virtual ~AnimationState();
	void Reset(const AnimationClip* clip);
	void SetWeight(float weight) { mWeight = weight; }
	void SetTime(float time) { mElapsedTime = time; }
	void AddTime(float time) { mElapsedTime += time; }
public:
	bool IsValid() const { return mClip != nullptr; }
	const AnimationClip* GetClip() const { return mClip; }
	float GetTime() const { return mElapsedTime; }
	float GetLength() const { return mClipLength; }
	float GetWeight() const { return mWeight; }
private:
	const AnimationClip* mClip;
	float mElapsedTime;
	float mClipLength;
	float mWeight;
};
