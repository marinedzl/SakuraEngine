#pragma once

class AnimationClip : public TRefObject<RefObject>
{
public:
	virtual ~AnimationClip();
	virtual bool LoadFromFile(const char* filename);
public:
	AnimationClip();
	size_t GetFrameCount() const { return mFrames.size(); }
	int GetFrameRate() const { return mFrameRate; }
	void GetTM(XMMATRIX& dst, float time, size_t index) const;
	float GetLength() const { return mLength; }
private:
	struct Frame;
	const Frame* GetFrame(size_t index) const { return mFrames[index]; }
private:
	size_t mBoneCount;
	float mLength;
	float mInterval;
	int mFrameRate;
	std::vector<Frame*> mFrames;
};
