#pragma once

class Gizmo
{
public:
	Gizmo(const Transform& transform);
	virtual ~Gizmo();
	const Transform& GetTransform() const { return mTransform;  }
	const GizmoMesh* GetMesh() const { return mMesh; }
	void SetMesh(GizmoMesh* mesh);
private:
	Transform mTransform;
	GizmoMesh* mMesh;
};
