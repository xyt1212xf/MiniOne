#pragma once
#include "EngineHead.h"
#include "plugin.h"
namespace mini
{
	class CPluginFactory : public TSingle<CPluginFactory>
	{
	public:
		CPluginFactory();
		virtual ~CPluginFactory();
		CPlugin*	createPlugin(PluginType type);

	private:
		class CScriptPlugin*	mpHandPlugin;
	};
}
