#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include "Interface.h"

namespace SECore
{
	SECORE_API bool InitCore(const char* resourePath);
	SECORE_API void ReleaseCore();
	SECORE_API Scene* CreateScene(void);
	SECORE_API RenderTarget* CreateRenderTarget(HWND hWnd);
}
