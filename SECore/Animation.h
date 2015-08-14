#pragma once
#include "AnimationBlender.h"

class Animation : public SECore::Animation
{
public:
	virtual ~Animation();
	virtual void Play(const char* clipname);
	virtual void CrossFade(const char* clipname, float fadeLength);
public:
	Animation(SceneEntity& owner);
	void Update(float deltaTime);
	void SetSkeleton(Skeleton* skeleton);
	bool AddClip(const char* name, AnimationClip* clip);
	bool GetMatrix(Matrix* dst) const;
	const AnimationClip* GetClip(const char* name) const;
	bool GetBoneTM(const char* name, Matrix& mat) const;
private:
	typedef std::map<std::string, AnimationClip*> Clips;
private:
	SceneEntity& mOwner;
	Skeleton* mSkeleton;
	Clips mClips;
	std::vector<Matrix> mTMs;
	std::vector<Matrix> mBones;
	AnimationBlender mBlender;
};
