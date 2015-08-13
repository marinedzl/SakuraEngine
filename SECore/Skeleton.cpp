#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

bool Skeleton::LoadFromFile(const char * filename)
{
	bool ret = false;
	const Vector3 scaling(1, 1, 1);
	buffer buff;

	CHECK(LoadBinaryFile(buff, filename));

	const char* data = (const char*)buff.ptr();
	SkeletonFile::Head* head = (SkeletonFile::Head*)data;
	data += sizeof(SkeletonFile::Head);

	mBoneCount = head->boneCount;

	mInverseTMs.resize(mBoneCount);

	for (size_t i = 0; i < mBoneCount; ++i)
	{
		const char* name = (const char*)data;
		data += sizeof(char) * SkeletonFile::MaxBoneName;

		mBoneNames.insert(std::make_pair(name, i));

		const Vector3& pos = *(const Vector3*)data;
		data += sizeof(Vector3);

		const Quat& rot = *(const Quat*)data;
		data += sizeof(Quat);

		XMVECTOR p = XMLoadFloat3((const XMFLOAT3*)&pos);
		XMVECTOR q = XMLoadFloat4((const XMFLOAT4*)&rot);
		XMVECTOR s = XMLoadFloat3((const XMFLOAT3*)&scaling);
		XMMATRIX m = XMMatrixAffineTransformation(s, XMQuaternionIdentity(), q, p);
		m = XMMatrixInverse(nullptr, m);
		XMStoreFloat4x4((XMFLOAT4X4*)&mInverseTMs[i], m);
	}

	ret = true;
Exit0:
	return ret;
}

UINT Skeleton::GetBoneID(const char * name) const
{
	BoneNames::const_iterator iter = mBoneNames.find(name);
	return iter == mBoneNames.end() ? 0 : iter->second;
}
