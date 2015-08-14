#include "stdafx.h"
#include "Skeleton.h"
#include "AnimationClip.h"
#include "AnimationContainer.h"

AnimationContainer::AnimationContainer()
	: mSkeleton(nullptr)
{
}

AnimationContainer::~AnimationContainer()
{
	SAFE_RELEASE(mSkeleton);
	ReleaseMap(mClips);
}

const AnimationClip * AnimationContainer::GetClip(const char * name) const
{
	Clips::const_iterator iter = mClips.find(name);
	return iter == mClips.end() ? nullptr : iter->second;
}

bool AnimationContainer::AddClip(const char* name, AnimationClip* clip)
{
	clip->AddRef();
	mClips.insert(std::make_pair(name, clip));
	return true;
}

void AnimationContainer::SetSkeleton(Skeleton * skeleton)
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

bool AnimationContainer::GetMatrix(Matrix* dst) const
{
	dst[0] = Matrix();
	memcpy(&dst[1], &mTMs[0], sizeof(Matrix) * mTMs.size());
	return true;
}

bool AnimationContainer::GetBoneTM(const char* name, Matrix& mat) const
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

void AnimationContainer::Blend(const BlendDesc& desc)
{
	if (desc.nextClip)
	{
		size_t boneCount = mSkeleton->GetBoneCount();
		for (size_t i = 0; i < boneCount; ++i)
		{
			XMMATRIX inv, mat, matNext;
			inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

			desc.currClip->GetTM(mat, desc.currTime, i);
			desc.nextClip->GetTM(matNext, desc.nextTime, i);

			mat = mat * (1 - desc.lerp) + matNext * desc.lerp;

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

			desc.currClip->GetTM(mat, desc.currTime, i);

			XMStoreFloat4x4(mBones[i], mat);

			mat = inv * mat;

			XMStoreFloat4x4(mTMs[i], mat);
		}
	}
}
