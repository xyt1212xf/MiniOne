#pragma once
#include "Wnd.h"
#include "GuiPlugin.h"

class CConsoleWnd : public mini::ui::CWnd
{
public:
	CConsoleWnd();
	virtual ~CConsoleWnd();

protected:
	virtual void bindMsg() override;
	bool play();

};
