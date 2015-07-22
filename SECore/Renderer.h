#pragma once

class SceneEntity;
class RenderEntity;
class RendererImpl : public Renderer
{
public:
	RendererImpl(SceneEntity& owner);
	virtual ~RendererImpl();
public:
	virtual void ClearEntities();
	virtual Entity* CreateEntity();
	virtual size_t GetEntityCount();
	virtual Entity* GetEntity(size_t index);
private:
	typedef std::vector<RenderEntity*> Entities;
private:
	SceneEntity& mOwner;
	Entities mEntities;
};
