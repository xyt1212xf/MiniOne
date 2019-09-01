#pragma once
#include "Wnd.h"
#include "GuiPlugin.h"

class CToolWnd : public mini::ui::CWnd
{
public:
	CToolWnd();
	virtual ~CToolWnd();
	void  initWnd();

protected:
	virtual void bindMsg() override;
	bool play();
	bool pause();
	bool step();
};

