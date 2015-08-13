#include "stdafx.h"
#include "Core.h"
#include "ShapeMesh.h"

const PxU32 g_numSlices = 2;  // along lines of longitude
const PxU32 g_numStacks = 16; // along lines of latitude

const PxU32 g_numSphereVertices = (g_numSlices * 2 + 1)*(g_numStacks + 1);
const PxU32 g_numSphereIndices = g_numSlices * 2 * g_numStacks * 2; //

const PxU32 g_numConeVertices = (g_numSlices * 2 + 1) * 2;
const PxU32 g_numConeIndices = g_numSlices * 2 * 2; //

PxVec3 g_spherePositions[g_numSphereVertices];
PxU16 g_sphereIndices[g_numSphereIndices];

PxVec3 g_conePositions[g_numConeVertices];
PxU16 g_coneIndices[g_numConeIndices];

// total vertex count
const PxU32 g_numCapsuleVertices = 2 * g_numSphereVertices + g_numConeVertices;
const PxU32 g_numCapsuleIndices = 2 * g_numSphereIndices + g_numConeIndices;

void generateSphereMesh(PxU16 slices, PxU16 stacks, PxVec3* positions, PxU16* indices)
{
	const PxF32 thetaStep = PxPi / stacks;
	const PxF32 phiStep = PxTwoPi / (slices * 2);

	PxF32 theta = 0.0f;

	// generate vertices
	for (PxU16 y = 0; y <= stacks; ++y)
	{
		PxF32 phi = 0.0f;

		PxF32 cosTheta = PxCos(theta);
		PxF32 sinTheta = PxSin(theta);

		for (PxU16 x = 0; x <= slices * 2; ++x)
		{
			PxF32 cosPhi = PxCos(phi);
			PxF32 sinPhi = PxSin(phi);

			PxVec3 p(cosPhi*sinTheta, cosTheta, sinPhi*sinTheta);

			// write vertex
			*(positions++) = p;

			phi += phiStep;
		}

		theta += thetaStep;
	}

	const PxU16 numRingQuads = 2 * slices;
	const PxU16 numRingVerts = 2 * slices + 1;

	// add faces
	for (PxU16 y = 0; y < stacks; ++y)
	{
		for (PxU16 i = 0; i < numRingQuads; ++i)
		{
			// add a quad
			*(indices++) = (y + 0)*numRingVerts + i;
			*(indices++) = (y + 1)*numRingVerts + i;
			/*
			*(indices++) = (y + 1)*numRingVerts + i + 1;

			*(indices++) = (y + 1)*numRingVerts + i + 1;
			*(indices++) = (y + 0)*numRingVerts + i + 1;
			*(indices++) = (y + 0)*numRingVerts + i;
			*/
		}
	}
}

void generateConeMesh(PxU16 slices, PxVec3* positions, PxU16* indices, PxU16 offset)
{
	// generate vertices
	const PxF32 phiStep = PxTwoPi / (slices * 2);
	PxF32 phi = 0.0f;

	// generate two rings of vertices for the cone ends
	for (int i = 0; i < 2; ++i)
	{
		for (PxU16 x = 0; x <= slices * 2; ++x)
		{
			PxF32 cosPhi = PxCos(phi);
			PxF32 sinPhi = PxSin(phi);

			PxVec3 p(cosPhi, 0.0f, sinPhi);

			// write vertex
			*(positions++) = p;

			phi += phiStep;
		}
	}

	const PxU16 numRingQuads = 2 * slices;
	const PxU16 numRingVerts = 2 * slices + 1;

	// add faces
	for (PxU16 i = 0; i < numRingQuads; ++i)
	{
		// add a quad
		*(indices++) = offset + i;
		*(indices++) = offset + numRingVerts + i;
		/*
		*(indices++) = offset + numRingVerts + i + 1;

		*(indices++) = offset + numRingVerts + i + 1;
		*(indices++) = offset + i + 1;
		*(indices++) = offset + i;
		*/
	}
}

void generateCapsuleMesh()
{
	generateSphereMesh(g_numSlices, g_numStacks, g_spherePositions, g_sphereIndices);
	generateConeMesh(g_numSlices, g_conePositions, g_coneIndices, g_numSphereVertices * 2);
}

void setDimensions(GizmoMesh* mesh, PxF32 halfHeight, PxF32 radius0, PxF32 radius1)
{
	static bool meshValid = false;
	if (!meshValid)
	{
		generateCapsuleMesh();
		meshValid = true;
	}

	GizmoMesh::Vertex* _vertices = new GizmoMesh::Vertex[g_numCapsuleVertices];
	PxU16* _indices = new USHORT[g_numCapsuleIndices];

	PxU16* indices = _indices;

	PxU32 positionStride = sizeof(GizmoMesh::Vertex);
	PxVec3* positions = (PxVec3*)_vertices;

	const PxF32 radii[2] = { radius1, radius0 };
	const PxF32 offsets[2] = { halfHeight, -halfHeight };

	// write two copies of the sphere mesh scaled and offset appropriately
	for (PxU32 s = 0; s < 2; ++s)
	{
		const PxF32 r = radii[s];
		const PxF32 offset = offsets[s];

		for (PxU32 i = 0; i < g_numSphereVertices; ++i)
		{
			PxVec3 p = g_spherePositions[i] * r;
			p.y += offset;

			*positions = p;

			positions = (PxVec3*)(((PxU8*)positions) + positionStride);
		}
	}

	// calculate cone angle
	PxF32 cosTheta = 0.0f;

	if (halfHeight > 0.0f)
		cosTheta = (radius0 - radius1) / (halfHeight*2.0f);

	// scale factor for normals to avoid re-normalizing each time
	PxF32 nscale = PxSqrt(1.0f - cosTheta*cosTheta);

	for (PxU32 s = 0; s < 2; ++s)
	{
		const PxF32 y = radii[s] * cosTheta;
		const PxF32 r = PxSqrt(radii[s] * radii[s] - y*y);
		const PxF32 offset = offsets[s] + y;

		for (PxU32 i = 0; i < g_numConeVertices / 2; ++i)
		{
			PxVec3 p = g_conePositions[i] * r;
			p.y += offset;
			*positions = p;

			PxVec3 n = g_conePositions[i] * nscale;
			n.y = cosTheta;

			positions = (PxVec3*)(((PxU8*)positions) + positionStride);
		}
	}

	// first sphere
	for (PxU32 i = 0; i < g_numSphereIndices; ++i)
		indices[i] = g_sphereIndices[i];

	indices += g_numSphereIndices;

	// second sphere
	for (PxU32 i = 0; i < g_numSphereIndices; ++i)
		indices[i] = g_sphereIndices[i] + g_numSphereVertices;

	indices += g_numSphereIndices;

	// cone indices
	for (PxU32 i = 0; i < g_numConeIndices; ++i)
		indices[i] = g_coneIndices[i];

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA dataDesc;

	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	mesh->indexCount = g_numCapsuleIndices;
	mesh->primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	ZeroMemory(&dataDesc, sizeof(dataDesc));

	bufferDesc.ByteWidth = g_numCapsuleVertices * sizeof(GizmoMesh::Vertex);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dataDesc.pSysMem = _vertices;
	CHECK(SUCCEEDED(device->CreateBuffer(&bufferDesc, &dataDesc, &mesh->vb)));

	bufferDesc.ByteWidth = g_numCapsuleIndices * sizeof(USHORT);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dataDesc.pSysMem = _indices;
	CHECK(SUCCEEDED(device->CreateBuffer(&bufferDesc, &dataDesc, &mesh->ib)));

Exit0:
	delete _vertices;
	delete _indices;
}

GizmoMesh* ShapeMesh::CreateCapsule(float halfHeight, float radius)
{
	GizmoMesh* mesh = new GizmoMesh();
	setDimensions(mesh, halfHeight, radius, radius);
	return mesh;
}
