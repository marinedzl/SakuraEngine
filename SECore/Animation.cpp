#include "stdafx.h"
#include "Skeleton.h"
#include "SceneEntity.h"
#include "AnimationClip.h"
#include "Animation.h"

Animation::Animation(SceneEntity& owner)
	: mOwner(owner)
	, mSkeleton(nullptr)
	, mClip(nullptr)
	, mNextClip(nullptr)
	, mElapsedTime(0)
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
	mClip = GetClip(clipname);
	mNextClip = nullptr;
	mElapsedTime = 0;
}

void Animation::CrossFade(const char * clipname, float fade)
{
	mNextClip = GetClip(clipname);
	mFadeLength = fade;
	mFadeTime = 0;
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
	mElapsedTime += deltaTime;

	if (mClip)
	{
		if (!mNextClip)
		{
			size_t boneCount = mSkeleton->GetBoneCount();
			for (size_t i = 0; i < boneCount; ++i)
			{
				XMMATRIX inv, mat;
				inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

				mClip->GetTM(mat, mElapsedTime, i);

				XMStoreFloat4x4(mBones[i], mat);

				mat = inv * mat;

				XMStoreFloat4x4(mTMs[i], mat);
			}
		}
		else
		{
			mFadeTime += deltaTime;
			mFadeTime = min(mFadeTime, mFadeLength);

			float weight = mFadeTime / mFadeLength;

			size_t boneCount = mSkeleton->GetBoneCount();
			for (size_t i = 0; i < boneCount; ++i)
			{
				XMMATRIX inv, mat, matNext;
				inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

				mClip->GetTM(mat, mElapsedTime, i);
				mNextClip->GetTM(matNext, mElapsedTime, i);

				mat = mat * (1 - weight) + matNext * weight;

				XMStoreFloat4x4(mBones[i], mat);

				mat = inv * mat;

				XMStoreFloat4x4(mTMs[i], mat);
			}

			if (mFadeTime >= mFadeLength)
			{
				mClip = mNextClip;
				mNextClip = nullptr;
			}
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
