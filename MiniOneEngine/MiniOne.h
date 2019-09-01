#pragma once
#include "Engine.h"

#ifdef WIN32
#include <windows.h>
namespace mini
{
	extern CEngine* createEngine(HWND hWnd);
	extern void destoryEngine();
}
#endif
