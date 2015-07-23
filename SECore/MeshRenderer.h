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
	void Draw(IRenderer::Entity* entity);
private:
};

extern MeshRenderer gMeshRenderer;
