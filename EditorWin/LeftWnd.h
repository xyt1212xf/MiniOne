#pragma once
#include "Wnd.h"
#include "GuiPlugin.h"
#include "HierarchyWnd.h"
#include "TableWnd.h"
#include "ResourceManager.h"

class CLeftWnd : public mini::ui::CWnd
{
public:
	CLeftWnd();
	virtual ~CLeftWnd();
	void initWnd();

private:
	CHierarchyWnd*			mpHierarchyWnd = nullptr;
	mini::ui::CTableWnd*	mpTableWnd = nullptr;
};
