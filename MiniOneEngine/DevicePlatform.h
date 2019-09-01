#pragma once
#include "EngineHead.h"
#include <string>
#ifdef WIN32

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <d3dx11tex.h>
#endif

struct DeviceConfig
{
	bool		vsync;
	bool		fullScreen;
	int			videoCardMemory;
	int			screenWidth;
	int			screenHeight;
	int			cameraNear;
	int			cameraFar;
	int			fps;
	std::string videoCardInfo;
#ifdef WIN32
	HWND	hWnd;
#endif // WIN32	
};

