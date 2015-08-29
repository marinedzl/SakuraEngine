#pragma once

class SceneLoader
{
public:
	SceneLoader();
	virtual ~SceneLoader();
	static bool Load(SECore::Scene* scene, const TCHAR* filename);
};
