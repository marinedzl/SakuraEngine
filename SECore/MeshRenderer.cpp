#include "stdafx.h"
#include "Core.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderEntity.h"
#include "ConstantBufferManager.h"
#include "RenderStateManager.h"
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
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	CHECK(LoadBinaryFile(file, "Mesh.cso"));
	CHECK(SUCCEEDED(device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), file.ptr(), file.size(), &mInputLayout)));
	CHECK(SUCCEEDED(device->CreateVertexShader(file.ptr(), file.size(), nullptr, &mMeshVS)));

	CHECK(LoadBinaryFile(file, "SkinnedMesh.cso"));
	CHECK(SUCCEEDED(device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), file.ptr(), file.size(), &mSkinnedInputLayout)));
	CHECK(SUCCEEDED(device->CreateVertexShader(file.ptr(), file.size(), nullptr, &mSkinnedMeshVS)));

	// CreateDepthStencilState
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.StencilEnable = FALSE;
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		mDepthStencilState = gRenderStateManager.GetRenderState<ID3D11DepthStencilState>(desc);
	}

	// CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = FALSE;
		desc.IndependentBlendEnable = FALSE; // 不针对多个RenderTarget使用不同的混合状态
		desc.RenderTarget[0].BlendEnable = FALSE;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 即使是disable也要填这个参数

		mBlendState = gRenderStateManager.GetRenderState<ID3D11BlendState>(desc);
	}

	ret = true;
Exit0:
	return ret;
}

void MeshRenderer::Begin()
{
	if (ID3D11DeviceContext* context = gCore.GetContext())
	{
		float factor[4] = { 0 };
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->OMSetBlendState(mBlendState, factor, 0xFFFFFFFF);
		context->OMSetDepthStencilState(mDepthStencilState, 1);
		context->RSSetState(gRenderStateManager.GetRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID));
	}
}

void MeshRenderer::End()
{

}

void MeshRenderer::Draw(RenderEntity* entity)
{
	if (!entity)
		return;

	ID3D11DeviceContext* context = gCore.GetContext();
	if (!context)
		return;

	Mesh* mesh = entity->GetMeshInternal();
	Material* material = entity->GetMaterialInternal();

	if (!mesh || !material)
		return;

	CBModel* cbModel = gConstantBufferManager.GetBuffer<CBModel>();
	if (!cbModel)
		return;

	cbModel->MATRIX_M = entity->GetWorld();

	if (entity->IsSkinned())
	{
		entity->GetBoneMatrix(cbModel->MATRIX_M_SKIN);
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
		material->SetPass(i);
		mesh->Draw();
	}
}
