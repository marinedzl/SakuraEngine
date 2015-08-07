#pragma once

class GizmoRenderer
{
public:
	GizmoRenderer();
	virtual ~GizmoRenderer();
	virtual void Release();
	bool Init();
	void Begin();
	void End();
	void Draw(const Camera& camera, const Gizmo* entity);
private:
	ID3D11VertexShader* mVS;
	ID3D11PixelShader* mPS;
	ID3D11InputLayout* mInputLayout;
	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11Buffer* mCBVS;
};

extern GizmoRenderer gGizmosRenderer;
