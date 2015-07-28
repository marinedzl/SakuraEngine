#pragma once

class SceneLoader
{
public:
	SceneLoader();
	virtual ~SceneLoader();
	static bool Load(Scene* scene, const char* filename);
};
