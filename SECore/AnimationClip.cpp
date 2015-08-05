#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip()
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
			const Vector3& pos = *(const Vector3*)data;
			data += sizeof(Vector3);

			const Quat& rot = *(const Quat*)data;
			data += sizeof(Quat);

			XMVECTOR p = XMLoadFloat3((const XMFLOAT3*)&pos);
			XMVECTOR q = XMLoadFloat4((const XMFLOAT4*)&rot);
			XMVECTOR s = XMLoadFloat3((const XMFLOAT3*)&scaling);
			XMMATRIX m = XMMatrixAffineTransformation(s, XMQuaternionIdentity(), q, p);
			XMStoreFloat4x4((XMFLOAT4X4*)&frame->tm[j], m);
		}
	}

	ret = true;
Exit0:
	return ret;
}
