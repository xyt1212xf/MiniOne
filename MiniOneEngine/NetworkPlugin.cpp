#include "pch.h"
#include "NetworkPlugin.h"
namespace mini
{
	//CNetworkPlugin* TSingle<CNetworkPlugin>::mpSingle = nullptr;

	CNetworkPlugin::CNetworkPlugin(CScriptPlugin* pPlugin)
		: mpPlugin(pPlugin)
	{

	}

	bool CNetworkPlugin::onStart()
	{
		return true;
	}

	void CNetworkPlugin::onRun()
	{

	}

	void CNetworkPlugin::onShutDown()
	{

	}

	CNetworkPlugin::~CNetworkPlugin()
	{

	}
}
