#pragma once

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
	virtual IRenderer* GetRenderer();
	virtual IRenderer* CreateRenderer();
public:
	virtual void DestroyAnimation();
	virtual IAnimation* GetAnimation();
	virtual IAnimation* CreateAnimation();
private:
	Matrix mWorld;
	Renderer* mRenderer;
	Animation* mAnimation;
};
