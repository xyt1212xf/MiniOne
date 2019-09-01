
#include "stdafx.h"
#include "mainfrm.h"
#include "SceneView.h"
#include "Resource.h"
#include "Editor.h"
#include "PropertiesWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView
CSceneView* gSceneView = nullptr;
CSceneView::CSceneView() noexcept
{
	gSceneView = this;
	mpChooseEntity = nullptr;
}

CSceneView::~CSceneView()
{
	for (auto item : mEntitys)
	{
		SafeRelease(item.second);
	}
}

BEGIN_MESSAGE_MAP(CSceneView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_CLICK, IDC_SCENE_TREECTRL, OnNMClick)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

void CSceneView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CSceneView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	UINT uFlag; 
	HTREEITEM hTree;
	GetCursorPos(&point); 
	m_wndSceneView.ScreenToClient(&point); 
	hTree = m_wndSceneView.HitTest(point, &uFlag); 

	if (hTree && (TVHT_ONITEM & uFlag))
	{
		CString name = m_wndSceneView.GetItemText(hTree);
		BOOL bCheck = m_wndSceneView.GetCheck(hTree);
		HTREEITEM hRoot = m_wndSceneView.GetRootItem();
		if (hRoot != hTree)
		{
			auto item = mEntitys.find(hTree);
			if( item != mEntitys.end() )
			{
				mpChooseEntity = item->second;
				if (uFlag & TVHT_ONITEMSTATEICON)
				{
					if (TRUE == bCheck)
					{
						(item->second)->hide();
					}
					else
					{
						(item->second)->show();
					}
				}
			}
		}
		else
		{
			mpChooseEntity = nullptr;
			if (uFlag & TVHT_ONITEMSTATEICON)
			{
				if (TRUE == bCheck)
				{
					GEngine->disableRendererPlugin();
				}
				else
				{
					GEngine->enableRendererPlugin();
				}
			}
		}
		gPropertiesWnd->setChooseEntity(mpChooseEntity);
	}

	*pResult = 0;
}

int CSceneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES;

	if (!m_wndSceneView.Create(dwViewStyle, rectDummy, this, IDC_SCENE_TREECTRL))
	{
		TRACE0("Failed to create scene view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_SceneViewImages.Create(IDB_LEVEL_ICON, 32, 0, RGB(0, 0, 0));
	m_wndSceneView.SetImageList(&m_SceneViewImages, TVSIL_NORMAL);

	OnChangeVisualStyle();

	AdjustLayout();

	return 0;
}

void CSceneView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CSceneView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndSceneView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CSceneView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndSceneView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CSceneView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndSceneView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CSceneView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndSceneView.SetFocus();
}

void CSceneView::OnChangeVisualStyle()
{
	return;
}

void CSceneView::insertItem(CEntity* pEntity)
{
	HTREEITEM hRoot = m_wndSceneView.GetRootItem();
	INT32 nSize = m_wndSceneView.GetCount();
	pEntity->AddRef();
	insertItem(pEntity->getEntityType(), pEntity->getName(), pEntity);
}

void CSceneView::insertItem(EntityType nType, const std::string& strContent, CEntity* pEntity)
{
	HTREEITEM hRoot = m_wndSceneView.GetRootItem();
	HTREEITEM hSelectItem = nullptr;
	switch (nType)
	{
	case Level:
	{
		if (hRoot == nullptr)
		{
			hSelectItem = m_wndSceneView.InsertItem(strContent.c_str(), 0, 0);
			m_wndSceneView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
		}
		else
		{
			hSelectItem = hRoot;
			m_wndSceneView.SetItemText(hRoot, strContent.c_str());
		}
	}
	break;
	case Actor:
	case Charactor:
	case SkyLight:
	case DirectionLight:
	case PlayStart:
	case Camera:
	{
		hSelectItem = m_wndSceneView.InsertItem(strContent.c_str(), nType, nType, hRoot);
	}
	break;
	default:
		break;
	}
	mEntitys[hSelectItem] = pEntity;
	m_wndSceneView.SetCheck(hSelectItem);
	m_wndSceneView.Expand(hRoot, TVE_EXPAND);
	m_wndSceneView.RedrawWindow();
}


CEntity* CSceneView::getChooseEntity()
{
	return nullptr;
}



void CSceneView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//block message, can't move here code
}
