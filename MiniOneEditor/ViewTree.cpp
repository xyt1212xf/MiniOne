
#include "stdafx.h"
#include "ViewTree.h"
#include "MiniOneEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree() noexcept
{
	mMessage = 0;
}

void CViewTree::SetCustomMsgBegin(UINT32 uBegin)
{
	if (mMessage == 0 )
	{
		AfxMessageBox(TEXT("Custom messages must be greater than zero"));
	}
	else
	{
		mMessage = uBegin;
	}
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
//	ON_NOTIFY_REFLECT(NM_CLICK, &CViewTree::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != nullptr);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

/*
void CViewTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CPoint curPoint;
	UINT nFlags;
	HTREEITEM hItem;

	GetCursorPos(&curPoint); //当前点坐标
	ScreenToClient(&curPoint); //屏幕坐标转换为TREE内坐标
	hItem = HitTest(curPoint, &nFlags); //坐标是否有ITEM

	if (hItem && (TVHT_ONITEM & nFlags)) //判断是否有HTREEITEM
	{
		HTREEITEM childrenitem = GetChildItem(hItem);
		if (childrenitem == nullptr)
		{
			theApp.setClickItem(GetItemText(hItem));
			theApp.clickEvent(mMessage);
		}
	}
	*pResult = 0;
}
*/



