#pragma once
#include "Wnd.h"
#include "TableWnd.h"
#include "GuiPlugin.h"
class CCreateEntityWnd : public mini::ui::CWnd
{
public:
	CCreateEntityWnd();
	virtual ~CCreateEntityWnd();

protected:
	virtual void bindMsg() override;

private:
	bool createCube();
};
