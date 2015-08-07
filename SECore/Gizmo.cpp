#include "stdafx.h"
#include "Gizmo.h"

Gizmo::Gizmo(const Transform& transform)
	: mTransform(transform)
	, mMesh(nullptr)
{
}

Gizmo::~Gizmo()
{
	SetMesh(nullptr);
}

void Gizmo::SetMesh(GizmoMesh* mesh)
{
	if (mMesh)
	{
		SAFE_RELEASE(mMesh->vb);
		SAFE_RELEASE(mMesh->ib);
		delete mMesh;
	}
	mMesh = mesh;
}
