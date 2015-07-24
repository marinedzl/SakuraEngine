#ifdef SEEXPORTOR_EXPORTS
#define SEEXPORTOR_API __declspec(dllexport)
#else
#define SEEXPORTOR_API __declspec(dllimport)
#endif

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;
