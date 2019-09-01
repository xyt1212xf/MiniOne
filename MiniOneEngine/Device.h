#pragma once
#ifdef WIN32
#include "DeviceDx11.h"
#elif OPENGLES
#include "DeviceOpenGLES.h"
#endif //  
namespace mini
{
#ifdef WIN32
	class CDevice : public CDeviceDx11
#elif OPENGLES
	class CDevice : public CDeviceOpenGLES
#endif //  
	{
	public:
		CDevice();
		virtual ~CDevice();
		DeviceConfig&	getConfig();

	private:
		DeviceConfig	mConfig;
	};
}



