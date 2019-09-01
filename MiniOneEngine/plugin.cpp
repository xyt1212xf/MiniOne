#include "pch.h"
#include "plugin.h"
namespace mini
{
	CPlugin::CPlugin()
	{
		mActive = true;
	}

	CPlugin::~CPlugin()
	{

	}

	bool CPlugin::initialize()
	{
		return true;
	}

	bool CPlugin::onStart()
	{
		return true;
	}

	void CPlugin::onRun()
	{

	}

	void CPlugin::onShutDown()
	{

	}

	void CPlugin::Process(event& msg)
	{

	}

	void CPlugin::active(bool bActive)
	{
		mActive = bActive;
	}

	void CPlugin::stop()
	{
		mbRun = false;
	}

	void CPlugin::resume()
	{
		mbRun = true;
	}

	bool CPlugin::isRun()
	{
		return mbRun;
	}

}
