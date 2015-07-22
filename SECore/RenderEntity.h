#pragma once

class SceneEntity;
class RenderEntity : public Renderer::Entity
{
public:
	RenderEntity(SceneEntity& owner);
	virtual ~RenderEntity();
public:
	virtual Mesh* GetMesh();
	virtual Material* GetMaterial();
	virtual void SetMesh(Mesh* mesh);
	virtual void SetMaterial(Material* material);
	virtual const Matrix& GetWorld() const;
	virtual void SetWorld(const Matrix& m);
private:
	SceneEntity& mOwner;
};
