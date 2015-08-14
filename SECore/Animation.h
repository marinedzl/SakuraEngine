#pragma once

class AnimationState;
class Animation : public SECore::Animation
{
public:
	virtual ~Animation();
	virtual void Play(const char* clipname);
	virtual void CrossFade(const char* clipname, float fadeLength);
	virtual bool GetBoneTM(const char* name, Matrix& mat) const;
public:
	Animation();
	void Update(float deltaTime);
	void SetSkeleton(Skeleton* skeleton);
	bool AddClip(const char* name, AnimationClip* clip);
	bool GetMatrix(Matrix* dst) const;
	const AnimationClip* GetClip(const char* name) const;
private:
	void UpdateTM();
private:
	typedef std::map<std::string, AnimationClip*> Clips;
private:
	Skeleton* mSkeleton;
	Clips mClips;
	std::vector<Matrix> mTMs;
	std::vector<Matrix> mBones;
	float mFadeLength;
	float mFadeElapsedTime;
	AnimationState* mCurrentState;
	AnimationState* mNextState;
};
