#include "stdafx.h"
#include "Skeleton.h"
#include "AnimationState.h"
#include "AnimationClip.h"
#include "Animation.h"

Animation::Animation()
	: mSkeleton(nullptr)
{
	mCurrentState = new AnimationState();
	mNextState = new AnimationState();
}

Animation::~Animation()
{
	SAFE_RELEASE(mSkeleton);
	ReleaseMap(mClips);
	SAFE_DELETE(mCurrentState);
	SAFE_DELETE(mNextState);
}

void Animation::Play(const char* clipname)
{
	mCurrentState->Reset(GetClip(clipname));
	mNextState->Reset(nullptr);
}

void Animation::CrossFade(const char * clipname, float fadeLength)
{
	mFadeLength = fadeLength;
	mFadeElapsedTime = 0;
	mNextState->Reset(GetClip(clipname));
}

void Animation::SetSkeleton(Skeleton * skeleton)
{
	SAFE_RELEASE(mSkeleton);
	mSkeleton = skeleton;
	if (mSkeleton)
	{
		mSkeleton->AddRef();
		mTMs.resize(mSkeleton->GetBoneCount());
		mBones.resize(mSkeleton->GetBoneCount());
	}
}

bool Animation::GetMatrix(Matrix* dst) const
{
	dst[0] = Matrix();
	memcpy(&dst[1], &mTMs[0], sizeof(Matrix) * mTMs.size());
	return true;
}

const AnimationClip * Animation::GetClip(const char * name) const
{
	Clips::const_iterator iter = mClips.find(name);
	return iter == mClips.end() ? nullptr : iter->second;
}
bool Animation::AddClip(const char* name, AnimationClip* clip)
{
	clip->AddRef();
	mClips.insert(std::make_pair(name, clip));
	return true;
}

bool Animation::GetBoneTM(const char* name, Matrix& mat) const
{
	bool ret = false;
	UINT id = mSkeleton->GetBoneID(name);
	if (id < 0)
		goto Exit0;
	mat = mBones[id];
	ret = true;
Exit0:
	return ret;
}

void Animation::Update(float deltaTime)
{
	if (!mCurrentState->IsValid())
		return;

	if (mNextState->IsValid())
	{
		mFadeElapsedTime += deltaTime;

		mCurrentState->AddTime(deltaTime);
		mNextState->AddTime(deltaTime);

		if (mFadeElapsedTime >= mFadeLength)
		{
			*mCurrentState = *mNextState;
			mNextState->Reset(nullptr);
		}
		else
		{
			float lerp = mFadeElapsedTime / mFadeLength;
			mCurrentState->SetWeight(1 - lerp);
			mNextState->SetWeight(lerp);
		}
	}
	else
	{
		float time = fmod(mCurrentState->GetTime() + deltaTime, mCurrentState->GetLength());
		mCurrentState->SetTime(time);
	}

	UpdateTM();
}

void Animation::UpdateTM()
{
	if (!mCurrentState->IsValid())
		return;

	if (mNextState->IsValid())
	{
		size_t boneCount = mSkeleton->GetBoneCount();
		for (size_t i = 0; i < boneCount; ++i)
		{
			XMMATRIX inv, mat, matNext;
			inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

			mCurrentState->GetClip()->GetTM(mat, mCurrentState->GetTime(), i);
			mNextState->GetClip()->GetTM(matNext, mNextState->GetTime(), i);

			mat = mat * mCurrentState->GetWeight() + matNext * mNextState->GetWeight();

			XMStoreFloat4x4(mBones[i], mat);

			mat = inv * mat;

			XMStoreFloat4x4(mTMs[i], mat);
		}
	}
	else
	{
		size_t boneCount = mSkeleton->GetBoneCount();
		for (size_t i = 0; i < boneCount; ++i)
		{
			XMMATRIX inv, mat;
			inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

			mCurrentState->GetClip()->GetTM(mat, mCurrentState->GetTime(), i);

			XMStoreFloat4x4(mBones[i], mat);

			mat = inv * mat;

			XMStoreFloat4x4(mTMs[i], mat);
		}
	}
}
