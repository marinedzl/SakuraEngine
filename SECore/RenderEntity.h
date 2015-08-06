#pragma once

class RenderEntity : public SECore::Renderer::Entity
{
public:
	RenderEntity(SceneEntity& owner);
	virtual ~RenderEntity();
public:
	virtual SECore::Mesh* GetMesh();
	virtual SECore::Material* GetMaterial();
	virtual void SetMesh(SECore::Mesh* mesh);
	virtual const Matrix& GetWorld() const;
	virtual void SetWorld(const Matrix& m);
	virtual bool IsSkinned() const;
public:
	bool GetBoneMatrix(Matrix* dst) const;
	virtual Mesh* GetMeshInternal() { return mMesh; }
	virtual Material* GetMaterialInternal() { return mMaterial; }
private:
	SceneEntity& mOwner;
	Material* mMaterial;
	Mesh* mMesh;
};
