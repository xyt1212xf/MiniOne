#include "stdafx.h"
#include "BottomWnd.h"

CBottomWnd::CBottomWnd()
{
	mpProjectWnd = new CProjectWnd();
	mpConsoleWnd = new CConsoleWnd();
}

CBottomWnd::~CBottomWnd()
{
	SafeRelease(mpProjectWnd);
	SafeRelease(mpConsoleWnd);
}

void CBottomWnd::initWnd()
{
	mini::ui::CGuiPlugin* pGuiPlugin = mini::GETPLUGIN(mini::ui::CGuiPlugin, mini::PluginType::GUI);
	pGuiPlugin->initWnd(this, "resource/Editor/Gui/BottomWnd.xml");

	mpTableWnd = dynamic_cast<mini::ui::CTableWnd*>(findChildWnd("tableWnd"));
	pGuiPlugin->initWnd(mpProjectWnd, "resource/Editor/Gui/ProjectWnd.xml");
	pGuiPlugin->initWnd(mpConsoleWnd, "resource/Editor/Gui/ConsoleWnd.xml");

	mpTableWnd->addToTable(mpProjectWnd, "Project");
	mpTableWnd->addToTable(mpConsoleWnd, "Console");
}




