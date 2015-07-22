#pragma once

class SceneEntity;
class Scene : public IScene
{
public:
	Scene();
	virtual ~Scene();
public:
	virtual void Draw();
	virtual void Update(float deltaTime);
public:
	virtual void Release();
	virtual void ClearEntities();
	virtual Entity* CreateEntity();
	virtual void RemoveEntity(Entity* entity);
private:
	typedef std::list<SceneEntity*> Entities;
private:
	Entities mEntities;
};
