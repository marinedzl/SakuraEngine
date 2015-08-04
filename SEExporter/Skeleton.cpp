#include "stdafx.h"
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

			IGameControl * pGameControl = node->GetIGameControl();

			GMatrix matWorld = node->GetWorldTM();

			Utility::ToVec3(bone->position, matWorld.Translation());
			Utility::ToVec4(bone->orientation, matWorld.Rotation());
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

	void Skeleton::WriteToFile(const TCHAR * path)
	{
		Json::Value json;
		size_t count = mBones.size();
		for (size_t i = 0; i < count; ++i)
		{
			json.append(WStr2MStr(mBones[i]->name));
		}
		MaxPlugin::SaveJsonToFile(path, json);
	}

	void Skeleton::LoadFromFile(const TCHAR * path)
	{
		Clear();
		TString buff;
		Json::Value root;
		MaxPlugin::LoadJsonFromFile(path, root);
		CHECK(root.isArray());
		uint count = root.size();
		for (uint i = 0; i < count; ++i)
		{
			const Json::Value& jsonBone = root[i];
			Bone* bone = new Bone();
			bone->id = i + 1;
			bone->name = MStr2WStr(root[i].asCString());
			mBones.push_back(bone);
			mBoneMap.insert(std::make_pair(bone->name, bone));
		}
	Exit0:
		;
	}
}
