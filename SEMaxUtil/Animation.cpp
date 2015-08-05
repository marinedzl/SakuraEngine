#include "stdafx.h"
#include "SECore/FileFormat.h"
#include "Skeleton.h"
#include "Animation.h"

namespace MaxPlugin
{
	Animation::Animation()
		: mFrameRate(60)
	{
	}

	Animation::~Animation()
	{
		Clear();
	}

	void Animation::Clear()
	{
		DeleteList(mFrames);
	}

	bool Animation::Extract(IGameScene * gScene)
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

	void Animation::ExtractNode(IGameNode * gNode)
	{
		DumpBone(gNode);

		for (int i = 0; i < gNode->GetChildCount(); i++)
			ExtractNode(gNode->GetNodeChild(i));
	}

	void Animation::DumpBone(IGameNode * node)
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
			DumpAnim(node->GetIGameControl(), Skeleton::Instance().GetBoneId(name) - 1);
		}
		break;
		}
	}

	bool Animation::DumpAnim(IGameControl * pGameControl, uint index)
	{
		bool ret = false;
		IGameKeyTab Key;

		CHECK((pGameControl->IsAnimated(IGAME_POS)) ||
			pGameControl->IsAnimated(IGAME_ROT) ||
			pGameControl->IsAnimated(IGAME_SCALE));
		
		CHECK(pGameControl->GetFullSampledKeys(Key, 1, IGAME_TM, false));

		int count = Key.Count();
		if (mFrames.size() == 0)
		{
			mFrames.resize(count);
			for (size_t i = 0; i < count; ++i)
			{
				mFrames[i] = new Frame();
				mFrames[i]->tm.resize(Skeleton::Instance().GetBoneCount());
			}
		}
		else
		{
			CHECK(count == mFrames.size());
		}

		for (int i = 0; i < count; i++)
		{
			Utility::ToVec3(mFrames[i]->tm[index].pos, Key[i].sampleKey.gval.Translation());
			Utility::ToVec4(mFrames[i]->tm[index].rot, Key[i].sampleKey.gval.Rotation());
		}

	Exit0:
		return ret;
	}

	void Animation::WriteFile(const TCHAR * filename)
	{
		AnimationFile::Head head;
		FILE* file = nullptr;

		_wfopen_s(&file, filename, _T("wb"));
		CHECK(file);

		head.version = 100;
		head.frameCount = (uint)mFrames.size();
		head.boneCount = Skeleton::Instance().GetBoneCount();

		fwrite(&head, sizeof(MeshFile::Head), 1, file);

		for (uint i = 0; i < head.frameCount; ++i)
		{
			const Frame* frame = mFrames[i];
			fwrite(&frame->tm[0], sizeof(frame->tm[0]), head.boneCount, file);
		}
	Exit0:
		if (file)
			fclose(file);
	}
}
