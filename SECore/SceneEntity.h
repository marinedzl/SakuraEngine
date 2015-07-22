#pragma once

class AnimationImpl;
class RendererImpl;
class SceneEntity : public IScene::Entity
{
public:
	SceneEntity();
	virtual ~SceneEntity();
public:
	virtual void SetWorld(const Matrix& m);
	virtual const Matrix& GetWorld() const;
public:
	virtual void Update(float deltaTime);
public:
	virtual void DestroyRenderer();
	virtual Renderer* GetRenderer();
	virtual Renderer* CreateRenderer();
public:
	virtual void DestroyAnimation();
	virtual Animation* GetAnimation();
	virtual Animation* CreateAnimation();
private:
	Matrix mWorld;
	RendererImpl* mRenderer;
	AnimationImpl* mAnimation;
};
