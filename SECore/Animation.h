#pragma once

class Animation : public IAnimation
{
public:
	Animation(SceneEntity& owner);
	virtual ~Animation();
public:
	void Update(float deltaTime);
public:
	virtual void Play(const char* clipname);
	virtual bool GetMatrix(Matrix* dst, size_t count) const;
private:
	SceneEntity& mOwner;
};
