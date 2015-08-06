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

void GetClipTM(XMMATRIX& dst, const AnimationClip* clip, float time, size_t index)
{
	const float interval = 1.0f / clip->GetFrameRate();
	int frameCount = clip->GetFrameCount();
	const float length = interval * frameCount;
	time = fmod(time, length);
	int frame = (int)(time / interval);
	float lerp = (time - frame * interval) / interval;

	const AnimationClip::Frame* prev = clip->GetFrame((frameCount + frame - 1) % frameCount);
	const AnimationClip::Frame* next = clip->GetFrame(frame % frameCount);

	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR prevT = XMLoadFloat3(prev->tm[index].pos);
	XMVECTOR prevR = XMLoadFloat4(prev->tm[index].rot);
	XMVECTOR nextT = XMLoadFloat3(next->tm[index].pos);
	XMVECTOR nextR = XMLoadFloat4(next->tm[index].rot);

	if (lerp <= 0)
	{
		position = prevT;
		rotation = prevR;
	}
	else if (lerp >= 1)
	{
		position = nextT;
		rotation = nextR;
	}
	else
	{
		position = XMVectorLerp(prevT, nextT, lerp);
		rotation = XMQuaternionSlerp(prevR, nextR, lerp);
	}

	XMMATRIX trans;
	trans = XMMatrixTranslationFromVector(position);
	dst = XMMatrixRotationQuaternion(rotation);
	dst *= trans;
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

				GetClipTM(mat, mClip, mElapsedTime, i);

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

				GetClipTM(mat, mClip, mElapsedTime, i);
				GetClipTM(matNext, mNextClip, mElapsedTime, i);

				mat = mat * (1 - weight) + matNext * weight;

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
