#pragma once

class AnimationClip : public TRefObject<RefObject>
{
public:
	virtual ~AnimationClip();
	virtual bool LoadFromFile(const char* filename);
public:
	struct Frame
	{
		std::vector<Matrix> tm;
	};
public:
	AnimationClip();
	size_t GetFrameSize() const { return mFrames.size(); }
	const Frame* GetFrame(size_t index) const { return mFrames[index]; }
private:
	size_t mBoneCount;
	std::vector<Frame*> mFrames;
};
