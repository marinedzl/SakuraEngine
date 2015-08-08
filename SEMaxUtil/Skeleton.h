#pragma once
#include "Utility.h"

namespace MaxPlugin
{
	struct Bone 
	{
		uint id;
		TString name;
		Vec3 position;
		Vec4 orientation;
		Vec3 scale;
	};

	class Skeleton
	{
	public:
		Skeleton();
		virtual ~Skeleton();
		void Clear();
		static Skeleton& Instance() { static Skeleton instance; return instance; }
		bool Build(IGameScene* gScene);
		uint GetBoneId(const TCHAR* name);
		void WriteToFile(const TCHAR* filename);
		void LoadFromFile(const TCHAR* filename);
		uint GetBoneCount() const { return (uint)mBones.size(); }
	private:
		void ExtractNode(IGameNode* gNode);
		void DumpBone(IGameNode * node);
		Bone* FindBone(const TCHAR* name);
	private:
		typedef std::map<TString, Bone*> BoneMap;
	private:
		std::vector<Bone*> mBones;
		BoneMap mBoneMap;
	};
}
