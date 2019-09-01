#pragma once
#include "plugin.h"
#include "ScriptHead.h"
#include "luaBind.h"
namespace mini
{
	class CScriptPlugin : public CPlugin, public TSingle<CScriptPlugin>
	{
		friend class CPluginFactory;
	public:
		virtual ~CScriptPlugin();
		virtual bool onStart();
		virtual void onRun();
		virtual void onShutDown();
		lua_State*	getLuaState();

		std::map<std::string, int>&	getGraphicDeviceConfig();

	private:
		CScriptPlugin();
		virtual bool initialize();
		void plugInConfig();
		void graphicConfig();

	private:
		CLuaBind*	mpLuaBind;
		lua_State *	mpLua;
		std::list<PluginType>		mNeedInitialisePlugin;
		std::map<std::string, int>	mGraphicDeviceConfig;
	};

}

