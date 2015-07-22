#pragma once

class SceneEntity;
class AnimationImpl : public Animation
{
public:
	AnimationImpl(SceneEntity& owner);
	virtual ~AnimationImpl();
public:
	void Update(float deltaTime);
public:
	virtual void Play(const char* clipname);
	virtual bool GetMatrix(Matrix* dst, size_t count) const;
private:
	SceneEntity& mOwner;
};
