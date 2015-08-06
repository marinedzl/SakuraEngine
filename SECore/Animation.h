#pragma once

class Animation : public IAnimation
{
public:
	virtual ~Animation();
	virtual void Play(const char* clipname);
	virtual void CrossFade(const char* clipname, float fade);
public:
	Animation(SceneEntity& owner);
	void Update(float deltaTime);
	void SetSkeleton(Skeleton* skeleton);
	bool AddClip(const char* name, AnimationClip* clip);
	bool GetMatrix(Matrix* dst) const;
	const AnimationClip* GetClip(const char* name) const;
private:
	typedef std::map<std::string, AnimationClip*> Clips;
private:
	SceneEntity& mOwner;
	Skeleton* mSkeleton;
	Clips mClips;
	const AnimationClip* mClip;
	float mElapsedTime;
	std::vector<Matrix> mTMs;
};
