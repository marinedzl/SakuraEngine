#pragma once

class SceneEntity : public IScene::Entity
{
public:
	SceneEntity();
	virtual ~SceneEntity();
public:
	virtual void SetWorld(const Matrix& m);
	virtual const Matrix& GetWorld() const;

	virtual void Update(float deltaTime);

	virtual void DestroyRenderer();
	virtual IRenderer* GetRenderer();
	virtual IRenderer* CreateRenderer();

	virtual void DestroyAnimation();
	virtual IAnimation* GetAnimation();
	virtual IAnimation* CreateAnimation();

	virtual void SetName(const char* name) { mName = name; }
	virtual const char* GetName() const { return mName.c_str(); }
private:
	std::string mName;
	Matrix mWorld;
	Renderer* mRenderer;
	Animation* mAnimation;
};
