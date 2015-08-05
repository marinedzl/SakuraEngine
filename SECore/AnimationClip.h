#pragma once

class AnimationClip : public TRefObject<RefObject>
{
public:
	virtual ~AnimationClip();
	virtual bool LoadFromFile(const char* filename);
public:
	struct Frame
	{
		struct TM
		{
			Vector3 pos;
			Quat rot;
		};
		std::vector<TM> tm;
	};
public:
	AnimationClip();
	size_t GetFrameCount() const { return mFrames.size(); }
	const Frame* GetFrame(size_t index) const { return mFrames[index]; }
	int GetFrameRate() const { return mFrameRate; }
private:
	size_t mBoneCount;
	int mFrameRate;
	std::vector<Frame*> mFrames;
};
