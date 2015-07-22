#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include "Interface.h"

namespace SECore
{
	SECORE_API Scene* CreateScene(void);
}
