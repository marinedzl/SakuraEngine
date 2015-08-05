#pragma once
#include "Utility.h"

namespace MaxPlugin
{
	struct Frame 
	{
		struct TM
		{
			Vec3 pos;
			Vec4 rot;
		};
		std::vector<TM> tm;
	};
	class Animation
	{
	public:
		Animation();
		virtual ~Animation();
		void Clear();
		bool Extract(IGameScene* gScene);
		void WriteFile(const TCHAR* filename);
	private:
		void ExtractNode(IGameNode * gNode);
		void DumpBone(IGameNode * gNode);
		bool DumpAnim(IGameControl * pGameControl, uint index);
	private:
		int mFrameRate;
		std::vector<Frame*> mFrames;
	};
}
