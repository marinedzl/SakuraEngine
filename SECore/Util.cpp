#include "stdafx.h"
#include "Util.h"

float dot(const Vector3& v1, const Vector3& v2)
{
	float ret = 0;
	XMVECTOR _v1 = XMLoadFloat3((XMFLOAT3*)&v1);
	XMVECTOR _v2 = XMLoadFloat3((XMFLOAT3*)&v2);
	XMVECTOR _r = XMVector3Dot(_v1, _v2);
	XMStoreFloat(&ret, _r);
	return ret;
}

bool TestRayOBBIntersection(
	Vector3 ray_origin, // Ray origin, in world space
	Vector3 ray_direction, // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	Vector3 aabb_min, // Minimum X,Y,Z coords of the mesh when not transformed at all.
	Vector3 aabb_max, // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	const Matrix& ModelMatrix, // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	) {

	// Intersection method from Real-Time Rendering and Essential Mathematics for Games

	float tMin = 0.0f;
	float tMax = 100000.0f;

	Vector3 OBBposition_worldspace(ModelMatrix.v[3].x, ModelMatrix.v[3].y, ModelMatrix.v[3].z);

	Vector3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		Vector3 xaxis(ModelMatrix.v[0].x, ModelMatrix.v[0].y, ModelMatrix.v[0].z);
		float e = dot(xaxis, delta);
		float f = dot(ray_direction, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
			float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
											 // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											 // We want t1 to represent the nearest intersection, 
											 // so if it's not the case, invert t1 and t2
			if (t1>t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		Vector3 yaxis(ModelMatrix.v[1].x, ModelMatrix.v[1].y, ModelMatrix.v[1].z);
		float e = dot(yaxis, delta);
		float f = dot(ray_direction, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.y) / f;
			float t2 = (e + aabb_max.y) / f;

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		Vector3 zaxis(ModelMatrix.v[2].x, ModelMatrix.v[2].y, ModelMatrix.v[2].z);
		float e = dot(zaxis, delta);
		float f = dot(ray_direction, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.z) / f;
			float t2 = (e + aabb_max.z) / f;

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;
}
