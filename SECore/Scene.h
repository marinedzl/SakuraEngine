#pragma once

class Scene : public IScene
{
	friend class SceneLoader;
public:
	Scene();
	virtual ~Scene();
public:
	virtual bool LoadAdditive(const char* filename);
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
