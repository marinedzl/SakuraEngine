#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "Skeleton.h"

namespace MaxPlugin
{
	Skeleton::Skeleton()
	{
	}

	Skeleton::~Skeleton()
	{
		Clear();
	}

	void Skeleton::Clear()
	{
		mBoneMap.clear();
		DeleteList(mBones);
	}

	bool Skeleton::Build(IGameScene* gScene)
	{
		bool ret = false;
		CHECK(gScene);
		Clear();

		int count = gScene->GetTopLevelNodeCount();
		for (int i = 0; i < count; ++i)
			ExtractNode(gScene->GetTopLevelNode(i));

		ret = true;
	Exit0:
		return ret;
	}
	
	void Skeleton::ExtractNode(IGameNode* gNode)
	{
		DumpBone(gNode);

		for (int i = 0; i < gNode->GetChildCount(); i++)
			ExtractNode(gNode->GetNodeChild(i));
	}

	void Skeleton::DumpBone(IGameNode* node)
	{
		IGameObject * obj = node->GetIGameObject();
		IGameObject::MaxType T = obj->GetMaxType();
		IGameObject::ObjectTypes type = obj->GetIGameType();
		const TCHAR * name = node->GetName();

		switch (type)
		{
		case IGameObject::IGAME_BONE:
		case IGameObject::IGAME_HELPER:
		{
			Bone * bone = new Bone();

			bone->id = (uint)mBones.size() + 1;
			bone->name = node->GetName();

			GMatrix matWorld = node->GetWorldTM();

			Utility::ToVec3(bone->position, matWorld.Translation());
			Utility::ToQuat(bone->orientation, matWorld.Rotation());
			Utility::ToVec3(bone->scale, matWorld.Scaling());

			mBones.push_back(bone);
			mBoneMap.insert(std::make_pair(bone->name, bone));
		}
		break;
		}
	}

	Bone* Skeleton::FindBone(const TCHAR * name)
	{
		BoneMap::iterator iter = mBoneMap.find(name);
		return iter == mBoneMap.end() ? nullptr : iter->second;
	}

	uint Skeleton::GetBoneId(const TCHAR* name)
	{
		if (Bone* bone = FindBone(name))
			return bone->id;
		else
			return 0;
	}

	void Skeleton::WriteToFile(const TCHAR * filename)
	{
		char buff[SkeletonFile::MaxBoneName];
		SkeletonFile::Head head;
		FILE* file = nullptr;

		_wfopen_s(&file, filename, _T("wb"));
		CHECK(file);

		head.version = 100;
		head.boneCount = GetBoneCount();

		fwrite(&head, sizeof(head), 1, file);

		for (uint i = 0; i < head.boneCount; ++i)
		{
			const Bone* bone = mBones[i];
			std::string name = WStr2MStr(bone->name);
			CHECK(bone->name.length() < SkeletonFile::MaxBoneName);
			ZeroMemory(buff, SkeletonFile::MaxBoneName);
			strncpy_s(buff, name.c_str(), SkeletonFile::MaxBoneName);
			fwrite(buff, sizeof(char), SkeletonFile::MaxBoneName, file);
			fwrite(&bone->position, sizeof(bone->position), 1, file);
			fwrite(&bone->orientation, sizeof(bone->orientation), 1, file);
		}
	Exit0:
		if (file)
			fclose(file);
	}

	void Skeleton::LoadFromFile(const TCHAR * filename)
	{
		Clear();
		char buff[SkeletonFile::MaxBoneName];
		SkeletonFile::Head head;
		FILE* file = nullptr;

		_wfopen_s(&file, filename, _T("rb"));
		CHECK(file);

		head.version = 100;
		head.boneCount = GetBoneCount();

		fread(&head, sizeof(head), 1, file);

		for (uint i = 0; i < head.boneCount; ++i)
		{
			Bone* bone = new Bone();
			fread(buff, sizeof(char), SkeletonFile::MaxBoneName, file);
			bone->name = MStr2WStr(buff);
			fread(&bone->position, sizeof(bone->position), 1, file);
			fread(&bone->orientation, sizeof(bone->orientation), 1, file);
			bone->id = (uint)mBones.size() + 1;
			mBones.push_back(bone);
			mBoneMap.insert(std::make_pair(bone->name, bone));
		}
	Exit0:
		if (file)
			fclose(file);
	}
}
