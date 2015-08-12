#include "stdafx.h"
#include "Core.h"
#include "ShapeMesh.h"

GizmoMesh* ShapeMesh::CreateBox(const Vector3& halfSize)
{
	GizmoMesh* mesh = nullptr;

	const Color c(0, 1, 0, 1);

	float vertices[] = {
		-halfSize.x, halfSize.y, -halfSize.z, c.r, c.g, c.b, c.a,
		halfSize.x, halfSize.y, -halfSize.z, c.r, c.g, c.b, c.a,
		halfSize.x, -halfSize.y, -halfSize.z, c.r, c.g, c.b, c.a,
		-halfSize.x, -halfSize.y, -halfSize.z, c.r, c.g, c.b, c.a,
		-halfSize.x, halfSize.y, halfSize.z, c.r, c.g, c.b, c.a,
		halfSize.x, halfSize.y, halfSize.z, c.r, c.g, c.b, c.a,
		halfSize.x, -halfSize.y, halfSize.z, c.r, c.g, c.b, c.a,
		-halfSize.x, -halfSize.y, halfSize.z, c.r, c.g, c.b, c.a,
	};

	unsigned short indices[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7,
	};

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA dataDesc;

	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	mesh = new GizmoMesh();
	CHECK(mesh);

	mesh->indexCount = _countof(indices);
	mesh->primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	ZeroMemory(&dataDesc, sizeof(dataDesc));

	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dataDesc.pSysMem = vertices;
	CHECK(SUCCEEDED(device->CreateBuffer(&bufferDesc, &dataDesc, &mesh->vb)));

	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dataDesc.pSysMem = indices;
	CHECK(SUCCEEDED(device->CreateBuffer(&bufferDesc, &dataDesc, &mesh->ib)));

Exit0:
	return mesh;
}
