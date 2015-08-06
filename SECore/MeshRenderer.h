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
	void Draw(RenderEntity* entity);
private:
	ID3D11VertexShader* mMeshVS;
	ID3D11VertexShader* mSkinnedMeshVS;
	ID3D11InputLayout* mInputLayout;
	ID3D11InputLayout* mSkinnedInputLayout;

	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;
};

extern MeshRenderer gMeshRenderer;
