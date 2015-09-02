#include "stdafx.h"
#include "Core.h"
#include "Camera.h"
#include "RenderStateManager.h"
#include "Gizmo.h"
#include "GizmoRenderer.h"

struct CBVS
{
	Matrix mvp;
	Color color;
};

GizmoRenderer gGizmosRenderer;

GizmoRenderer::GizmoRenderer()
	: mVS(nullptr)
	, mPS(nullptr)
	, mInputLayout(nullptr)
	, mCBVS(nullptr)
{
}

GizmoRenderer::~GizmoRenderer()
{
}

void GizmoRenderer::Release()
{
	SAFE_RELEASE(mVS);
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mInputLayout);
	SAFE_RELEASE(mCBVS);
}

bool GizmoRenderer::Init()
{
	bool ret = false;
	ID3D11Device* device = gCore.GetDevice();
	CHECK(device);

	// CreateShader
	{
		buffer file;
		const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CHECK(LoadBinaryFile(file, "GizmoVS.cso"));
		CHECK(SUCCEEDED(device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), file.ptr(), file.size(), &mInputLayout)));
		CHECK(SUCCEEDED(device->CreateVertexShader(file.ptr(), file.size(), nullptr, &mVS)));

		CHECK(LoadBinaryFile(file, "GizmoPS.cso"));
		CHECK(SUCCEEDED(device->CreatePixelShader(file.ptr(), file.size(), nullptr, &mPS)));
	}

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

	// CreateConstantBuffer
	{
		D3D11_BUFFER_DESC buffDesc;
		ZeroMemory(&buffDesc, sizeof(buffDesc));
		buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDesc.Usage = D3D11_USAGE_DYNAMIC;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffDesc.ByteWidth = sizeof(CBVS);
		CHECK(SUCCEEDED(device->CreateBuffer(&buffDesc, nullptr, &mCBVS)));
	}

	ret = true;
Exit0:
	return ret;
}

void GizmoRenderer::Begin()
{
	ID3D11DeviceContext* context = gCore.GetContext();
	if (!context)
		return;

	float factor[4] = { 0 };
	context->OMSetBlendState(mBlendState, factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(mDepthStencilState, 1);
	context->RSSetState(gRenderStateManager.GetRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID));
	context->VSSetShader(mVS, nullptr, 0);
	context->PSSetShader(mPS, nullptr, 0);
	context->IASetInputLayout(mInputLayout);
	context->VSSetConstantBuffers(0, 1, &mCBVS);
}

void GizmoRenderer::End()
{

}

void GizmoRenderer::Draw(const Camera& camera, const Gizmo* entity)
{
	if (!entity)
		return;

	ID3D11DeviceContext* context = gCore.GetContext();
	if (!context)
		return;

	D3D11_MAPPED_SUBRESOURCE mr;

	if (SUCCEEDED(context->Map(mCBVS, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
	{
		CBVS* cb = (CBVS*)mr.pData;
		XMMATRIX vp =  camera.GetView() * camera.GetProj();
		XMMATRIX m, parent;
		AffineTransform(m, entity->GetLocal());
		AffineTransform(parent, entity->GetOwner());
		m *= parent;
		m *= vp;
		cb->color = entity->GetColor();
		XMStoreFloat4x4(cb->mvp, m);
		context->Unmap(mCBVS, 0);
	}

	const GizmoMesh* mesh = entity->GetMesh();
	UINT stride = sizeof(GizmoMesh::Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mesh->vb, &stride, &offset);
	context->IASetIndexBuffer(mesh->ib, DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(mesh->primitive);
	context->DrawIndexed(mesh->indexCount, 0, 0);
}
