#pragma once

class SceneEntity : public SECore::Scene::Entity
{
public:
	virtual ~SceneEntity();
	virtual void SetWorld(const Matrix& m);
	virtual const Matrix& GetWorld() const;

	virtual void Update(float deltaTime);

	virtual void DestroyRenderer();
	virtual SECore::Renderer* GetRenderer();
	virtual SECore::Renderer* CreateRenderer();

	virtual void DestroyAnimation();
	virtual SECore::Animation* GetAnimation();
	virtual SECore::Animation* CreateAnimation();

	virtual void DestroyCollider();
	virtual SECore::Collider* GetCollider();
	virtual SECore::Collider* CreateCollider(Collider::Type type);

	virtual void SetName(const char* name) { mName = name; }
	virtual const char* GetName() const { return mName.c_str(); }
public:
	SceneEntity(Scene& scene);
	bool GetSkinMatrix(Matrix* dst) const;
	Scene& GetScene() { return mScene; }
	template<typename T> T* GetComponent() {}
	template<> Collider* GetComponent() { return mCollider; }
	template<> Animation* GetComponent() { return mAnimation; }
	template<> Renderer* GetComponent() { return mRenderer; }
private:
	std::string mName;
	Matrix mWorld;
	Scene& mScene;
	Renderer* mRenderer;
	Animation* mAnimation;
	Collider* mCollider;
};
