#include "stdafx.h"
#include "Core.h"
#include "SECore.h"

namespace SECore
{
	SECORE_API Core* CreateCore()
	{
		if (gCore.Init())
		{
			return &gCore;
		}

		return nullptr;
	}
}
