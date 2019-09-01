#include "stdafx.h"
#include "RightWnd.h"

CRightWnd::CRightWnd()
{

}

CRightWnd::~CRightWnd()
{

}

void CRightWnd::initWnd()
{
	mini::ui::CGuiPlugin* pGuiPlugin = mini::GETPLUGIN(mini::ui::CGuiPlugin, mini::PluginType::GUI);
	pGuiPlugin->initWnd(this, "resource/Editor/Gui/RightWnd.xml");

	mpTableWnd = dynamic_cast<mini::ui::CTableWnd*>(findChildWnd("tableWnd"));
//	pGuiPlugin->initWnd(mpHierarchyWnd, "resource/Editor/Gui/HierarchyWnd.xml");
}
