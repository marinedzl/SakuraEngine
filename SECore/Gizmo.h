#pragma once

class Gizmo
{
public:
	Gizmo();
	virtual ~Gizmo();
	const Transform& GetTransform() const { return mTransform;  }
	Transform& GetTransform() { return mTransform; }
	const GizmoMesh* GetMesh() const { return mMesh; }
	void SetMesh(GizmoMesh* mesh);
	virtual const Color& GetColor() const { return mColor; }
	virtual void SetColor(const Color& color) { mColor = color; }
private:
	Transform mTransform;
	GizmoMesh* mMesh;
	Color mColor;
};
