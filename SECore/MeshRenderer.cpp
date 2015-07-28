#include "stdafx.h"
#include "Core.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderEntity.h"
#include "ConstantBufferManager.h"
#include "MeshRenderer.h"

MeshRenderer gMeshRenderer;

MeshRenderer::MeshRenderer()
	: mMeshVS(nullptr)
	, mSkinnedMeshVS(nullptr)
	, mInputLayout(nullptr)
	, mSkinnedInputLayout(nullptr)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Release()
{
	SAFE_RELEASE(mMeshVS);
	SAFE_RELEASE(mSkinnedMeshVS);
	SAFE_RELEASE(mInputLayout);
	SAFE_RELEASE(mSkinnedInputLayout);
}

bool MeshRenderer::Init()
{
	bool ret = false;
	buffer file;
	const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	CHECK(LoadBinaryFile(file, "Mesh.cso"));
	CHECK(SUCCEEDED(device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), file.ptr(), file.size(), &mInputLayout)));
	CHECK(SUCCEEDED(device->CreateVertexShader(file.ptr(), file.size(), nullptr, &mMeshVS)));

	CHECK(LoadBinaryFile(file, "SkinnedMesh.cso"));
	CHECK(SUCCEEDED(device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), file.ptr(), file.size(), &mSkinnedInputLayout)));
	CHECK(SUCCEEDED(device->CreateVertexShader(file.ptr(), file.size(), nullptr, &mSkinnedMeshVS)));

	ret = true;
Exit0:
	return ret;
}

void MeshRenderer::Begin()
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void MeshRenderer::End()
{

}

void MeshRenderer::Draw(IRenderer::Entity* entity)
{
	if (!entity)
		return;

	ID3D11DeviceContext* context = gCore.GetContext();
	if (!context)
		return;

	Mesh* mesh = dynamic_cast<Mesh*>(entity->GetMesh());
	Material* material = dynamic_cast<Material*>(entity->GetMaterial());

	if (!mesh || !material)
		return;

	CBModel* cbModel = gConstantBufferManager.GetBuffer<CBModel>();
	if (!cbModel)
		return;

	cbModel->MATRIX_M = entity->GetWorld();

	size_t boneCount = mesh->GetBoneCount();
	if (boneCount > 0)
	{
		context->VSSetShader(mSkinnedMeshVS, nullptr, 0);
		context->IASetInputLayout(mSkinnedInputLayout);
	}
	else
	{
		context->VSSetShader(mMeshVS, nullptr, 0);
		context->IASetInputLayout(mInputLayout);
	}

	gConstantBufferManager.Commit<CBModel>();

	mesh->Setup();

	size_t passCount = material->GetPassCount();
	for (size_t i = 0; i < passCount; ++i)
	{
		material->Setup(i);
		mesh->Draw();
	}
}
