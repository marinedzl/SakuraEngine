#pragma once
#include <queue>
#include "AnimationContainer.h"

class Animation : public AnimationContainer
{
public:
	virtual ~Animation();
	virtual void Play(const char* name, bool loop = true);
	virtual void CrossFade(const char* name, float fadeLength, bool loop = true);
	virtual void CrossFadeQueue(const char* name, float offset, float fadeLength, bool loop = true);
	virtual void SetClipChangedCallback(ClipChangedCallback* callback, void* userData) { 
		mClipChangedCallback = callback, mClipChangedCallbackData = userData; }
public:
	Animation();
	virtual void Update(float deltaTime);
private:
	struct FadeData
	{
		const AnimationClip* clip;
		float offset;
		float length;
		bool loop;
	};
private:
	void ModTime();
	void CheckFadeQueue();
private:
	ClipChangedCallback* mClipChangedCallback;
	void* mClipChangedCallbackData;
	std::deque<FadeData> mFadeQueue;
	float mFadeLength;
	float mFadeElapsedTime;
	bool mIsLoop;
	BlendDesc mBlendDesc;
};
