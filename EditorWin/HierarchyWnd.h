#pragma once
#include "Wnd.h"
#include "TableWnd.h"
#include "GuiPlugin.h"
#include "CreateEntityWnd.h"
class CHierarchyWnd : public mini::ui::CWnd
{
public:
	CHierarchyWnd();
	virtual ~CHierarchyWnd();
	void initWnd();

protected:
	virtual void bindMsg() override;

private:
	bool create();

private:
	CCreateEntityWnd* mpCreateEntityWnd = nullptr;
};

