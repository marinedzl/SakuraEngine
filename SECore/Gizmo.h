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
private:
	Transform mTransform;
	GizmoMesh* mMesh;
};
