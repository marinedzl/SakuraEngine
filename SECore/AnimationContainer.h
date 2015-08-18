#pragma once

class AnimationContainer : public SECore::Animation
{
public:
	struct SavedBoneTM
	{
		size_t index;
		Matrix tm;
	};
	struct BlendDesc
	{
		const AnimationClip* currClip;
		const AnimationClip* nextClip;
		float currTime;
		float nextTime;
		float lerp;
	};
	virtual ~AnimationContainer();
	virtual void Blend(const BlendDesc& desc);
	virtual const AnimationClip* GetClip(const char* name) const;
	virtual bool AddSavedBoneTM(const char* bone);
	virtual bool GetSavedBoneTM(const char* bone, Matrix& dst) const;
public:
	AnimationContainer();
	void SetSkeleton(Skeleton* skeleton);
	bool AddClip(AnimationClip* clip);
	bool GetMatrix(Matrix* dst) const;
private:
	typedef std::map<std::string, AnimationClip*> Clips;
	typedef std::map<std::string, SavedBoneTM> SavedBoneTMs;
private:
	Skeleton* mSkeleton;
	Clips mClips;
	std::vector<Matrix> mTMs;
	SavedBoneTMs mSavedBoneTMs;
};
