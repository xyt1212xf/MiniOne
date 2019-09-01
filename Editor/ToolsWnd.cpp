
#include "stdafx.h"
#include "ToolsWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Editor.h"
#include "Foundation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
INT32 ButtonWidth = 90;

CToolsWnd* gToolsWnd = nullptr;
CToolsWnd::CToolsWnd() noexcept
{
	gToolsWnd = this;
	mbRun = false;
}

CToolsWnd::~CToolsWnd()
{
}

BEGIN_MESSAGE_MAP(CToolsWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	
	ON_COMMAND(IDC_TOOLS_RUN, OnClickedRunBtn)
	ON_UPDATE_COMMAND_UI(IDC_TOOLS_RUN, OnUpdateRunBtn)

	ON_COMMAND(IDC_TOOLS_RUN, OnClickedSaveBtn)
	ON_UPDATE_COMMAND_UI(IDC_TOOLS_SAVE, OnUpdateSaveBtn)

	ON_COMMAND(IDC_TOOLS_BUILD, OnClickedBuildBtn)
	ON_UPDATE_COMMAND_UI(IDC_TOOLS_BUILD, OnUpdateBuildBtn)

	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CToolsWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	m_SaveBtn.SetWindowPos(nullptr, 0, rectClient.top, ButtonWidth, rectClient.bottom, SWP_NOZORDER);
	m_BuildBtn.SetWindowPos(nullptr, ButtonWidth + 1, rectClient.top, ButtonWidth, rectClient.bottom, SWP_NOZORDER);
	m_RunBtn.SetWindowPos(nullptr, rectClient.Width() - ButtonWidth - 1, rectClient.top, ButtonWidth, rectClient.bottom, SWP_NOZORDER);
}

int CToolsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_RunBtn.Create(TEXT("Pause"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_TOOLS_RUN);
	m_SaveBtn.Create(TEXT("Save"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_TOOLS_SAVE);
	m_BuildBtn.Create(TEXT("Build"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_TOOLS_BUILD);
	AdjustLayout();
	return 0;
}

void CToolsWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
	RedrawWindow();
}


void CToolsWnd::OnClickedSaveBtn()
{

}

void CToolsWnd::OnClickedRunBtn()
{
	mbRun = !mbRun;
	if (mbRun)
	{
		m_RunBtn.SetWindowText("Run");
		GEngine->setStatus(Running);
	}
	else
	{
		m_RunBtn.SetWindowText("Pause");
		GEngine->setStatus(Editoring);
	}
}

void CToolsWnd::OnClickedBuildBtn()
{

}

void CToolsWnd::OnUpdateRunBtn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CToolsWnd::OnUpdateBuildBtn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CToolsWnd::OnUpdateSaveBtn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CToolsWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDockablePane::OnPaint() for painting messages
	CRect rc;
	GetClientRect(rc);
	CBrush   brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	dc.FillRect(&rc, &brush);
}


void CToolsWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//block message, can't move here code
}
