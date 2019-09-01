#include "stdafx.h"
#include "ToolWnd.h"


CToolWnd::CToolWnd()
{

}

CToolWnd::~CToolWnd()
{
}

void CToolWnd::initWnd()
{
	mini::ui::CGuiPlugin* pGuiPlugin = mini::GETPLUGIN(mini::ui::CGuiPlugin, mini::PluginType::GUI);
	pGuiPlugin->initWnd(this, "resource/Editor/Gui/ToolWnd.xml");
}

void CToolWnd::bindMsg()
{
	BindMessage(mini::LEFTUP, "play", &CToolWnd::play);
	BindMessage(mini::LEFTUP, "pause", &CToolWnd::pause);
	BindMessage(mini::LEFTUP, "step", &CToolWnd::step);
}

bool CToolWnd::play()
{
	return true;
}

bool CToolWnd::pause()
{
	return true;
}

bool CToolWnd::step()
{
	return true;
}

