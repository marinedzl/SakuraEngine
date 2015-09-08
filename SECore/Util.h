#pragma once

float dot(const Vector3& v1, const Vector3& v2);

bool TestRayOBBIntersection(
	Vector3 ray_origin,
	Vector3 ray_direction,
	Vector3 aabb_min,
	Vector3 aabb_max,
	const Matrix& ModelMatrix,
	float& intersection_distance
	);
