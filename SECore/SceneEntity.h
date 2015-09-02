#pragma once

class SceneEntity : public SECore::Scene::Entity
{
public:
	virtual ~SceneEntity();
	virtual Transform& GetTransform() { return mTransform; }
	virtual const Transform& GetTransform() const { return mTransform; }

	virtual void Update(float deltaTime);

	virtual void DestroyRenderer();
	virtual SECore::Renderer* GetRenderer();
	virtual SECore::Renderer* CreateRenderer();

	virtual void DestroyAnimation();
	virtual SECore::Animation* GetAnimation();
	virtual SECore::Animation* CreateAnimation();

	virtual void DestroyCollider();
	virtual SECore::Collider* GetCollider();
	virtual SECore::Collider* CreateBoxCollider(bool isDynamic, const Vector3& size);

	virtual void DestroyCCT();
	virtual SECore::CharacterController* GetCCT();
	virtual SECore::CharacterController* CreateCCT(float height, float radius, const Vector3& offset);

	virtual void SetName(const char* name) { mName = name; }
	virtual const char* GetName() const { return mName.c_str(); }

	const Bound& GetBound() const { return mBound; }
	void CalcBound();
public:
	SceneEntity(Scene& scene);
	bool GetSkinMatrix(Matrix* dst) const;
	Scene& GetScene() { return mScene; }
	template<typename T> T* GetComponent() {}
	template<> Collider* GetComponent() { return mCollider; }
	template<> Animation* GetComponent() { return mAnimation; }
	template<> Renderer* GetComponent() { return mRenderer; }
private:
	Bound mBound;
	std::string mName;
	Transform mTransform;
	Scene& mScene;
	Renderer* mRenderer;
	Animation* mAnimation;
	Collider* mCollider;
	CharacterController* mCCT;
};
