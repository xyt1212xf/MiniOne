Win32config = 
{
	screenWidth = 1280;
	screenHeight = 768;
}

DX11 = 
{
	fullScreen = 0;
	vsync = 1;
	deviceWidth = 1280;
	deviceHeight = 768;
	cameraNear = 1;
	cameraFar = 1000;
	fps			= 30;
}

Plugin = 
{
	RENDERER = 1;
	NETWORK = 0;
	GUI = 1;
}


require "shaderLists"

package.path = ".\";script\\?.lua"




