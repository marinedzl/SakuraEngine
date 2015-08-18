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

bool AnimationContainer::AddSavedBoneTM(const char * bone)
{
	SavedBoneTM tm = { mSkeleton->GetBoneID(bone), Matrix() };
	mSavedBoneTMs.insert(std::make_pair(bone, tm));
	return true;
}

bool AnimationContainer::GetSavedBoneTM(const char * bone, Matrix& dst) const
{
	SavedBoneTMs::const_iterator iter = mSavedBoneTMs.find(bone);
	if (iter == mSavedBoneTMs.end())
	{
		return false;
	}
	else
	{
		dst = iter->second.tm;
		return true;
	}
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

	static Matrix savedBones[MAX_BONE];

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

			XMStoreFloat4x4(savedBones[i], mat);

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

			XMStoreFloat4x4(savedBones[i], mat);

			mat = inv * mat;

			XMStoreFloat4x4(mTMs[i], mat);
		}
	}

	if (!mSavedBoneTMs.empty())
	{
		SavedBoneTMs::iterator iter = mSavedBoneTMs.begin();
		SavedBoneTMs::iterator iterEnd = mSavedBoneTMs.end();
		for (; iter != iterEnd; ++iter)
		{
			iter->second.tm = savedBones[iter->second.index];
		}
	}
}
