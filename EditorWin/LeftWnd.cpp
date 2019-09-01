#include "stdafx.h"
#include "LeftWnd.h"

CLeftWnd::CLeftWnd()
{
	mpHierarchyWnd = new CHierarchyWnd();
}

CLeftWnd::~CLeftWnd()
{
	SafeRelease(mpHierarchyWnd);
}

void CLeftWnd::initWnd()
{
	mini::ui::CGuiPlugin* pGuiPlugin = mini::GETPLUGIN(mini::ui::CGuiPlugin, mini::PluginType::GUI);
	pGuiPlugin->initWnd(this, "resource/Editor/Gui/LeftWnd.xml");
	
	mpTableWnd = dynamic_cast<mini::ui::CTableWnd*>(findChildWnd("tableWnd"));
	pGuiPlugin->initWnd(mpHierarchyWnd, "resource/Editor/Gui/HierarchyWnd.xml");
	
	mpTableWnd->addToTable(mpHierarchyWnd, "Hierarchy");

	mpHierarchyWnd->initWnd();
}
