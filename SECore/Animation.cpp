#include "stdafx.h"
#include "Skeleton.h"
#include "SceneEntity.h"
#include "AnimationClip.h"
#include "Animation.h"

Animation::Animation(SceneEntity& owner)
	: mOwner(owner)
	, mSkeleton(nullptr)
	, mClip(nullptr)
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
	mElapsedTime = 0;
}

void Animation::CrossFade(const char * clipname, float fade)
{
	mClip = GetClip(clipname);
	mElapsedTime = 0;
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
	if (mClip)
	{
		mElapsedTime += deltaTime;

		const float interval = 1.0f / mClip->GetFrameRate();
		int frameCount = mClip->GetFrameCount();
		const float length = interval * frameCount;
		float time = fmod(mElapsedTime, length);
		int frame = (int)(time / interval);
		float lerp = (time - frame * interval) / interval;

		const AnimationClip::Frame* prev = mClip->GetFrame((frameCount + frame - 1) % frameCount);
		const AnimationClip::Frame* next = mClip->GetFrame(frame % frameCount);
		
		size_t boneCount = mSkeleton->GetBoneCount();
		for (size_t i = 0; i < boneCount; ++i)
		{
			XMVECTOR position;
			XMVECTOR rotation;
			XMVECTOR prevT = XMLoadFloat3(prev->tm[i].pos);
			XMVECTOR prevR = XMLoadFloat4(prev->tm[i].rot);
			XMVECTOR nextT = XMLoadFloat3(next->tm[i].pos);
			XMVECTOR nextR = XMLoadFloat4(next->tm[i].rot);

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

			XMMATRIX mat, trans, inv;
			trans = XMMatrixTranslationFromVector(position);
			mat = XMMatrixRotationQuaternion(rotation);
			mat *= trans;

			inv = XMLoadFloat4x4((XMFLOAT4X4*)&mSkeleton->GetInverseTM(i));

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
