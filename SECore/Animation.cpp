#include "stdafx.h"
#include "AnimationClip.h"
#include "Animation.h"

Animation::Animation()
	: mClipChangedCallback(nullptr)
	, mClipChangedCallbackData(nullptr)
	, mIsLoop(false)
{
	mBlendDesc.currClip = nullptr;
	mBlendDesc.nextClip = nullptr;
	mBlendDesc.currTime = 0;
	mBlendDesc.nextTime = 0;
	mBlendDesc.lerp = 0;
}

Animation::~Animation()
{
}

float ModClipTime(float time, float length, bool isloop)
{
	if (isloop)
		return fmod(time, length);
	else
		return length;
}

void Animation::Update(float deltaTime)
{
	mBlendDesc.currTime += deltaTime;
	mBlendDesc.nextTime += deltaTime;

	if (!mBlendDesc.currClip)
		return;

	ModTime();

	if (mBlendDesc.nextClip)
	{
		mFadeElapsedTime += deltaTime;
		if (mFadeElapsedTime >= mFadeLength)
		{
			if (mClipChangedCallback)
			{
				mClipChangedCallback(mClipChangedCallbackData, mBlendDesc.currClip->GetName(), mBlendDesc.nextClip->GetName());
			}

			mFadeElapsedTime = 0;
			mBlendDesc.currClip = mBlendDesc.nextClip;
			mBlendDesc.currTime = mBlendDesc.nextTime;
			mBlendDesc.nextClip = nullptr;
		}
		else
		{
			mBlendDesc.lerp = mFadeElapsedTime / mFadeLength;
		}
	}

	Blend(mBlendDesc);

	CheckFadeQueue();
}

void Animation::Play(const char * name, bool loop)
{
	mFadeQueue.clear();
	mBlendDesc.currClip = GetClip(name);
	CHECK(mBlendDesc.currClip);
	mBlendDesc.currTime = 0;
	mBlendDesc.nextClip = nullptr;
	mIsLoop = loop;
Exit0:
	;
}

void Animation::CrossFade(const char * name, float fadeLength, bool loop)
{
	mFadeQueue.clear();
	mBlendDesc.nextClip = GetClip(name);
	mBlendDesc.nextTime = 0;
	CHECK(mBlendDesc.nextClip);
	mFadeLength = fadeLength;
	mFadeElapsedTime = 0;
	mBlendDesc.lerp = 0;
	mIsLoop = loop;
Exit0:
	;
}

void Animation::CrossFadeQueue(const char * name, float offset, float fadeLength, bool loop)
{
	FadeData data = { GetClip(name), offset, fadeLength, loop };
	CHECK(data.clip);
	mFadeQueue.push_back(data);
Exit0:
	;
}

void Animation::CheckFadeQueue()
{
	if (!mBlendDesc.nextClip)
	{
		if (!mFadeQueue.empty())
		{
			const FadeData& data = mFadeQueue.front();
			if (mBlendDesc.currTime >= data.offset * mBlendDesc.currClip->GetLength())
			{
				mBlendDesc.nextClip = data.clip;
				mBlendDesc.nextTime = 0;
				mFadeElapsedTime = 0;
				mFadeLength = data.length;
				mBlendDesc.lerp = 0;
				mIsLoop = data.loop;
				mFadeQueue.pop_front();
			}
		}
	}
}

void Animation::ModTime()
{
	float length = mBlendDesc.currClip->GetLength();
	if (mBlendDesc.currTime > length)
	{
		if (mIsLoop)
			mBlendDesc.currTime = fmod(mBlendDesc.currTime, length);
		else
			mBlendDesc.currTime = length;
	}
}
