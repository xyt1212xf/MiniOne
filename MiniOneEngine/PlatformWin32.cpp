#include "pch.h"
#include "MiniOne.h"
namespace mini
{
#ifdef WIN32

	CEngine* createEngine(HWND hWnd)
	{
		CEngine* pEngine = CEngine::initialize();
		pEngine->init(hWnd);
		GEngine = pEngine;
		return pEngine;
	}

	void destoryEngine()
	{
		CEngine::destory();
		GEngine = nullptr;
	}
#endif // WIN32
}

