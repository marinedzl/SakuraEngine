#pragma once

class RenderEntity : public IRenderer::Entity
{
public:
	RenderEntity(SceneEntity& owner);
	virtual ~RenderEntity();
public:
	virtual IMesh* GetMesh();
	virtual IMaterial* GetMaterial();
	virtual void SetMesh(IMesh* mesh);
	virtual const Matrix& GetWorld() const;
	virtual void SetWorld(const Matrix& m);
private:
	SceneEntity& mOwner;
	Material* mMaterial;
};
