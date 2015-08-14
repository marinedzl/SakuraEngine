#include "stdafx.h"
#include "Skeleton.h"
#include "SceneEntity.h"
#include "AnimationClip.h"
#include "Animation.h"

Animation::Animation(SceneEntity& owner)
	: mOwner(owner)
	, mSkeleton(nullptr)
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
		mTMs.resize(mSkeleton->GetBoneCount());
		mBones.resize(mSkeleton->GetBoneCount());
	}
}

void Animation::Play(const char* clipname)
{
	mBlender.Play(GetClip(clipname));
}

void Animation::CrossFade(const char * clipname, float fadeLength)
{
	mBlender.CrossFade(GetClip(clipname), fadeLength);
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

void Animation::Update(float deltaTime)
{
	mBlender.Update(deltaTime);

	const AnimationState* currState = mBlender.GetCurrentState();
	const AnimationState* nextState = mBlender.GetNextState();

	if (!currState)
		return;

	if (nextState)
	{
		size_t boneCount = mSkeleton->GetBoneCount();
		for (size_t i = 0; i < boneCount; ++i)
		{
			XMMATRIX inv, mat, matNext;
			inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

			currState->GetClip()->GetTM(mat, currState->GetTime(), i);
			nextState->GetClip()->GetTM(matNext, nextState->GetTime(), i);

			mat = mat * currState->GetWeight() + matNext * nextState->GetWeight();

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

			currState->GetClip()->GetTM(mat, currState->GetTime(), i);

			XMStoreFloat4x4(mBones[i], mat);

			mat = inv * mat;

			XMStoreFloat4x4(mTMs[i], mat);
		}
	}
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
