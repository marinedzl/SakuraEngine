#pragma once

class AnimationClip : public TRefObject<SECore::AnimationClip>
{
public:
	virtual ~AnimationClip();
	virtual float GetLength() const { return mLength; }
	virtual const char* GetName() const { return mName.c_str(); }
	virtual void SetName(const char* name) { mName = name; }
public:
	AnimationClip();
	size_t GetFrameCount() const { return mFrames.size(); }
	int GetFrameRate() const { return mFrameRate; }
	void GetTM(XMVECTOR& position, XMVECTOR& rotation, float time, size_t index) const;
	virtual bool LoadFromFile(const char* filename);
private:
	struct Frame;
	const Frame* GetFrame(size_t index) const { return mFrames[index]; }
private:
	std::string mName;
	size_t mBoneCount;
	float mLength;
	float mInterval;
	int mFrameRate;
	std::vector<Frame*> mFrames;
};
