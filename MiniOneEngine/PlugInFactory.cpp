#include "pch.h"
#include "PlugInFactory.h"
#include "ScriptPlugin.h"
#include "RendererPlugin.h"
#include "NetworkPlugin.h"
#include "GuiPlugin.h"
#include "Engine.h"

//CPluginFactory* TSingle<CPluginFactory>::mpSingle = nullptr;
namespace mini
{
	CPluginFactory::CPluginFactory()
	{

	}

	CPluginFactory::~CPluginFactory()
	{

	}

	CPlugin* CPluginFactory::createPlugin(PluginType type)
	{
		CPlugin* pPlugin = nullptr;
		switch (type)
		{
		case SCRIPT:
			pPlugin = new CScriptPlugin();
			mpHandPlugin = dynamic_cast<CScriptPlugin*>(pPlugin);
			break;
		case GUI:
			pPlugin = new ui::CGuiPlugin(mpHandPlugin);
			break;
		case NETWORK:
			pPlugin = new CNetworkPlugin(mpHandPlugin);
			break;
		case RENDERER:
			pPlugin = new CRendererPlugin(mpHandPlugin);
			break;
		}
		return pPlugin != nullptr ? (pPlugin->initialize() ? pPlugin : nullptr) : nullptr;
	}

}
