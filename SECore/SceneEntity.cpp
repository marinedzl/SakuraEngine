#include "stdafx.h"
#include "Renderer.h"
#include "Animation.h"
#include "BoxCollider.h"
#include "CharacterController.h"
#include "Scene.h"
#include "Gizmo.h"
#include "ShapeMesh.h"
#include "SceneEntity.h"

SceneEntity::SceneEntity(Scene& scene)
	: mScene(scene)
	, mGizmo(nullptr)
	, mRenderer(nullptr)
	, mAnimation(nullptr)
	, mCollider(nullptr)
	, mCCT(nullptr)
{
}

SceneEntity::~SceneEntity()
{
	DestroyAnimation();
	DestroyRenderer();
	DestroyCollider();
	DestroyCCT();
	if (mGizmo)
		mScene.RemoveGizmo(mGizmo);
	SAFE_DELETE(mGizmo);
}

bool SceneEntity::GetSkinMatrix(Matrix* dst) const
{
	return mAnimation->GetMatrix(dst);
}

void SceneEntity::DestroyRenderer()
{
	SAFE_DELETE(mRenderer);
}

SECore::Renderer* SceneEntity::GetRenderer()
{
	return mRenderer;
}

SECore::Renderer* SceneEntity::CreateRenderer()
{
	CHECK(!mRenderer);
	mRenderer = new Renderer(*this);
Exit0:
	return mRenderer;
}

void SceneEntity::DestroyAnimation()
{
	SAFE_DELETE(mAnimation);
}

SECore::Animation* SceneEntity::GetAnimation()
{
	return mAnimation;
}

SECore::Animation* SceneEntity::CreateAnimation()
{
	CHECK(!mAnimation);
	mAnimation = new Animation();
Exit0:
	return mAnimation;
}

void SceneEntity::DestroyCollider()
{
	delete mCollider;
}

SECore::Collider * SceneEntity::GetCollider()
{
	return mCollider;
}

SECore::Collider* SceneEntity::CreateBoxCollider(bool isDynamic, const Vector3& size)
{
	SAFE_DELETE(mCollider);
	BoxCollider* collider = new BoxCollider(*this);
	collider->SetSize(size);
	CHECK(collider->Init(isDynamic));
	mCollider = collider;
Exit0:
	return mCollider;
}

void SceneEntity::DestroyCCT()
{
	delete mCCT;
}

SECore::CharacterController* SceneEntity::GetCCT()
{
	return mCCT;
}

SECore::CharacterController* SceneEntity::CreateCCT(float height, float radius, const Vector3& offset)
{
	SAFE_DELETE(mCCT);
	CharacterController* cct = new CharacterController(*this);
	CHECK(cct->Init(mScene.GetCCTMgr(), height, radius, offset));
	mCCT = cct;
Exit0:
	return mCCT;
}

void SceneEntity::Update(float deltaTime)
{
	if (mCCT)
		mCCT->Update(deltaTime);
	if (mAnimation)
		mAnimation->Update(deltaTime);

	if (mGizmo)
	{
		mGizmo->GetTransform() = mTransform;
		mGizmo->GetTransform().position = mGizmo->GetTransform().position +mGizmoCenter;
	}
}

void SceneEntity::CalcBound()
{
	if (!mRenderer)
		return;

	mBound.max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	mBound.min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	size_t count = mRenderer->GetEntityCount();
	for (size_t i = 0; i < count; i++)
	{
		const Bound& bound = mRenderer->GetEntity(i)->GetMesh()->GetBound();
		_max(mBound.max, mBound.max, bound.max);
		_min(mBound.min, mBound.min, bound.min);
	}

	if (mGizmo)
	{
		mScene.RemoveGizmo(mGizmo);
		delete mGizmo;
		mGizmo = nullptr;
	}

	Vector3 size = (mBound.max - mBound.min) / 2;
	mGizmoCenter = mBound.min + size;
	mGizmo = new Gizmo();
	CHECK(mGizmo);
	mGizmo->SetMesh(ShapeMesh::CreateBox(size));
	mGizmo->SetColor(Color(0.5f, 0.5f, 0.5f, 1));
	mScene.AddGizmo(mGizmo);
Exit0:
	;
}
