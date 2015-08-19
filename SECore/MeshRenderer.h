#pragma once

class MeshRenderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();
	virtual void Release();
	bool Init();
	void Begin(bool drawingGBuffer = false);
	void End();
	void Draw(RenderEntity* entity);
private:
	enum Mode
	{
		eNormalPass,
		eGBufferPass,
		eModeCount
	};
private:
	ID3D11VertexShader* mMeshVS[eModeCount];
	ID3D11VertexShader* mSkinnedMeshVS[eModeCount];
	ID3D11InputLayout* mInputLayout;
	ID3D11InputLayout* mSkinnedInputLayout;

	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;

	Mode mMode;

	ID3D11Buffer* mCBModel;
};

extern MeshRenderer gMeshRenderer;
