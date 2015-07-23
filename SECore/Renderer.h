#pragma once

class Renderer : public IRenderer
{
public:
	Renderer(SceneEntity& owner);
	virtual ~Renderer();
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
