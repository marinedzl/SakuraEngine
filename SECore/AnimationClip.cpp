#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "AnimationClip.h"

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
