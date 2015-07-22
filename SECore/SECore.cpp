#include "stdafx.h"
#include "Scene.h"
#include "SECore.h"

namespace SECore
{
	SECORE_API Scene* CreateScene(void)
	{
		return new ::Scene();
	}
}
