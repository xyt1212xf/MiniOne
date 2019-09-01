#pragma once
#include "Wnd.h"
#include "GuiPlugin.h"
#include "TableWnd.h"
#include "ProjectWnd.h"
#include "ConsoleWnd.h"

class CBottomWnd : public mini::ui::CWnd
{
public:
	CBottomWnd();
	virtual ~CBottomWnd();
	void initWnd();

private:
	mini::ui::CTableWnd*	mpTableWnd = nullptr;
	CConsoleWnd*			mpConsoleWnd = nullptr;
	CProjectWnd*			mpProjectWnd = nullptr;
};
