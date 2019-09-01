
#pragma once
#include <list>
#include <map>
#include "ButtonEx.h"


class CToolsWnd : public CDockablePane
{
// Construction
public:
	CToolsWnd() noexcept;
	virtual ~CToolsWnd();
	virtual BOOL OnShowControlBarMenu(CPoint point) { return TRUE; }
	void AdjustLayout();

private:
	bool		mbRun;
	CButton		m_RunBtn;
	CButton		m_SaveBtn;
	CButton		m_BuildBtn;
// Implementation
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickedSaveBtn();
	afx_msg void OnClickedRunBtn();
	afx_msg void OnClickedBuildBtn();
	afx_msg void OnUpdateSaveBtn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunBtn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildBtn(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

extern CToolsWnd* gToolsWnd;
