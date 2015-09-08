#pragma once

float dot(const Vector3& v1, const Vector3& v2);

bool TestRayOBBIntersection(
	Vector3 ray_origin, // Ray origin, in world space
	Vector3 ray_direction, // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	Vector3 aabb_min, // Minimum X,Y,Z coords of the mesh when not transformed at all.
	Vector3 aabb_max, // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	const Matrix& ModelMatrix, // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	);
