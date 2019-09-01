#pragma once
#include "plugin.h"
#include "ScriptPlugin.h"
namespace mini
{
	class CNetworkPlugin : public CPlugin, public TSingle<CNetworkPlugin>
	{
		friend class CPluginFactory;
	private:
		CNetworkPlugin(CScriptPlugin* pPlugin);
		virtual bool onStart();
		virtual void onRun();
		virtual void onShutDown();

	public:
		virtual ~CNetworkPlugin();
	private:
		CScriptPlugin*	mpPlugin;
	};
}