#include "stdafx.h"
#include "HierarchyWnd.h"
#include "Engine.h"
#include "GuiPlugin.h"
#include "ResourceManager.h"
CHierarchyWnd::CHierarchyWnd()
{
	mpCreateEntityWnd = new CCreateEntityWnd;
}

CHierarchyWnd::~CHierarchyWnd()
{
	SafeRelease(mpCreateEntityWnd);
}

void CHierarchyWnd::initWnd()
{
	mini::ui::CGuiPlugin* pGuiPlugin = mini::GETPLUGIN(mini::ui::CGuiPlugin, mini::PluginType::GUI);
	pGuiPlugin->initWnd(mpCreateEntityWnd, "resource/Editor/Gui/CreateEntityWnd.xml");
	
	addChildWnd(mpCreateEntityWnd);
	mpCreateEntityWnd->setVisual(false);
}

void CHierarchyWnd::bindMsg()
{
	BindMessage(mini::LEFTUP, "Create", &CHierarchyWnd::create);
}

bool CHierarchyWnd::create()
{
	auto* pWnd = findChildWnd("Create");
	if (nullptr != pWnd)
	{
		mini::CPoint2F&& pt = pWnd->getLocalPosition();
		mini::CPoint2F&& size = pWnd->getSize();
		mpCreateEntityWnd->setPosition( pt.x, pt.y + size.y + 1);
		mpCreateEntityWnd->setVisual(true);
	}
	return true;
}



