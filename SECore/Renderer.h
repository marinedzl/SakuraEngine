#pragma once

class Renderer : public SECore::Renderer
{
public:
	virtual ~Renderer();
	virtual void ClearEntities();
	virtual Entity* CreateEntity();
	virtual size_t GetEntityCount();
	virtual Entity* GetEntity(size_t index);
public:
	Renderer(SceneEntity& owner);
	virtual RenderEntity* GetEntityInternal(size_t index);
private:
	typedef std::vector<RenderEntity*> Entities;
private:
	SceneEntity& mOwner;
	Entities mEntities;
};
