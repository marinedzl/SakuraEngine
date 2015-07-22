#include "stdafx.h"
#include "MeshRenderer.h"
#include "Core.h"

Core gCore;

Core::Core()
{
}

Core::~Core()
{
}

bool Core::Init()
{
	bool ret = false;
	CHECK(gMeshRenderer.Init());
	ret = true;
Exit0:
	return true;
}

void Core::Release()
{
	gMeshRenderer.Release();
}
