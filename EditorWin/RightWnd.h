#pragma once
#include "Wnd.h"
#include "GuiPlugin.h"
#include "TableWnd.h"

class CRightWnd : public mini::ui::CWnd
{
public:
	CRightWnd();
	virtual ~CRightWnd();
	void initWnd();

private:
	mini::ui::CTableWnd*	mpTableWnd = nullptr;
};
