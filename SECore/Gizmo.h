#pragma once

class Gizmo
{
public:
	Gizmo(const Transform& owner);
	virtual ~Gizmo();
	const Transform& GetLocal() const { return mLocal;  }
	Transform& GetLocal() { return mLocal; }
	const Transform& GetOwner() const { return mOwner; }
	const GizmoMesh* GetMesh() const { return mMesh; }
	void SetMesh(GizmoMesh* mesh);
	virtual const Color& GetColor() const { return mColor; }
	virtual void SetColor(const Color& color) { mColor = color; }
private:
	const Transform& mOwner;
	Transform mLocal;
	GizmoMesh* mMesh;
	Color mColor;
};
