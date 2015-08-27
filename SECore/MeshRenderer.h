#pragma once

class MeshRenderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();
	virtual void Release();
	bool Init();
	void Begin();
	void End();
	void Draw(RenderEntity* entity, RenderPass mode);
private:
	ID3D11VertexShader* mMeshVS[eRenderPassCount];
	ID3D11VertexShader* mSkinnedMeshVS[eRenderPassCount];
	ID3D11InputLayout* mInputLayout;
	ID3D11InputLayout* mSkinnedInputLayout;

	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;

	ID3D11Buffer* mCBModel;
};

extern MeshRenderer gMeshRenderer;
