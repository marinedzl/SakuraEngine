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

void AnimationContainer::SetSkeleton(Skeleton * skeleton)
{
	SAFE_RELEASE(mSkeleton);
	mSkeleton = skeleton;
	if (mSkeleton)
	{
		mSkeleton->AddRef();
		mTMs.resize(mSkeleton->GetBoneCount());
	}
}

bool AnimationContainer::GetMatrix(Matrix* dst) const
{
	dst[0] = Matrix();
	memcpy(&dst[1], &mTMs[0], sizeof(Matrix) * mTMs.size());
	return true;
}

const AnimationClip * AnimationContainer::GetClip(const char * name) const
{
	Clips::const_iterator iter = mClips.find(name);
	return iter == mClips.end() ? nullptr : iter->second;
}
bool AnimationContainer::AddClip(AnimationClip* clip)
{
	clip->AddRef();
	mClips.insert(std::make_pair(clip->GetName(), clip));
	return true;
}

void AnimationContainer::Blend(const BlendDesc& blendDesc)
{
	if (!blendDesc.currClip)
		return;

	if (blendDesc.nextClip)
	{
		size_t boneCount = mSkeleton->GetBoneCount();
		for (size_t i = 0; i < boneCount; ++i)
		{
			XMMATRIX inv, mat, matNext;
			inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

			((AnimationClip*)blendDesc.currClip)->GetTM(mat, blendDesc.currTime, i);
			((AnimationClip*)blendDesc.nextClip)->GetTM(matNext, blendDesc.nextTime, i);

			mat = mat * (1 - blendDesc.lerp) + matNext * blendDesc.lerp;

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

			((AnimationClip*)blendDesc.currClip)->GetTM(mat, blendDesc.currTime, i);

			mat = inv * mat;

			XMStoreFloat4x4(mTMs[i], mat);
		}
	}
}
