#pragma once

struct ShapeMesh
{
	static GizmoMesh* CreateBox(const Vector3& halfSize);
	static GizmoMesh* CreateCapsule(float halfHeight, float radius);
};
