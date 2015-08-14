#pragma once

class AnimationContainer : public SECore::Animator
{
public:
	virtual ~AnimationContainer();
	virtual bool GetBoneTM(const char* name, Matrix& mat) const;
public:
	struct BlendDesc
	{
		BlendDesc() : currClip(nullptr), nextClip(nullptr), currTime(0), nextTime(0), lerp(0) {}
		const AnimationClip* currClip;
		const AnimationClip* nextClip;
		float currTime;
		float nextTime;
		float lerp;
	};
public:
	AnimationContainer();
	bool AddClip(const char* name, AnimationClip* clip);
	const AnimationClip* GetClip(const char* name) const;
	void SetSkeleton(Skeleton* skeleton);
	bool GetMatrix(Matrix* dst) const;
protected:
	void Blend(const BlendDesc& desc);
private:
	typedef std::map<std::string, AnimationClip*> Clips;
private:
	Skeleton* mSkeleton;
	std::vector<Matrix> mTMs;
	std::vector<Matrix> mBones;
	Clips mClips;
};
