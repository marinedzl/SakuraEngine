#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	virtual ~BoxCollider();
	virtual void OnInitShape();
public:
	BoxCollider(SceneEntity& owner);
	void SetSize(const Vector3& size) { mSize = size; }
private:
	Vector3 mSize;
};
