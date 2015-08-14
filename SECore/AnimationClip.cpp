#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "AnimationClip.h"

struct AnimationClip::Frame
{
	struct TM
	{
		Vector3 pos;
		Quat rot;
	};
	std::vector<TM> tm;
};

AnimationClip::AnimationClip()
	: mBoneCount(0)
	, mFrameRate(30)
{
}

AnimationClip::~AnimationClip()
{
	DeleteList(mFrames);
}

bool AnimationClip::LoadFromFile(const char * filename)
{
	bool ret = false;
	const Vector3 scaling(1, 1, 1);
	buffer buff;

	CHECK(LoadBinaryFile(buff, filename));

	const char* data = (const char*)buff.ptr();
	AnimationFile::Head* head = (AnimationFile::Head*)data;
	data += sizeof(AnimationFile::Head);

	mBoneCount = head->boneCount;

	mFrames.resize(head->frameCount);

	for (size_t i = 0; i < head->frameCount; ++i)
	{
		Frame* frame = new Frame();
		mFrames[i] = frame;
		frame->tm.resize(head->boneCount);

		for (size_t j = 0; j < mBoneCount; ++j)
		{
			frame->tm[j].pos = *(const Vector3*)data;
			data += sizeof(Vector3);

			frame->tm[j].rot = *(const Quat*)data;
			data += sizeof(Quat);
		}
	}

	ret = true;
Exit0:
	return ret;
}

void AnimationClip::GetTM(XMMATRIX& dst, float time, size_t index) const
{
	const float interval = 1.0f / GetFrameRate();
	int frameCount = GetFrameCount();
	const float length = interval * frameCount;
	time = fmod(time, length);
	int frame = (int)(time / interval);
	float lerp = (time - frame * interval) / interval;

	const AnimationClip::Frame* prev = GetFrame((frameCount + frame - 1) % frameCount);
	const AnimationClip::Frame* next = GetFrame(frame % frameCount);

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
