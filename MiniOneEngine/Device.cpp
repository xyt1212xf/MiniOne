#include "pch.h"
#include "Device.h"
namespace mini
{
	CDevice::CDevice()
	{
	}


	CDevice::~CDevice()
	{
	}

	DeviceConfig& CDevice::getConfig()
	{
		return mConfig;
	}
}
