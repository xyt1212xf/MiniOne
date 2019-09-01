
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Editor.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_SETTEXT, &CMainFrame::OnSetText)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_pStatic = nullptr;
}

CMainFrame::~CMainFrame()
{
	SafeDelete(m_pStatic);
}

void CMainFrame::setClickItem(CString itemName)
{
	mItemName = itemName;
}


LRESULT CMainFrame::OnSetText(WPARAM wParam, LPARAM lParam)
{
	std::string strTitle = (char*)lParam;
	
	if (strTitle == "SceneWnd - MiniOne")
	{
		theApp.changeViewport(SceneWnd, "SceneWnd");
	}
	else if (strTitle == "MaterialWnd - MiniOne")
	{
		theApp.changeViewport(MaterialWnd, "MaterialWnd");
	}
	else if (strTitle == "MeshWnd - MiniOne")
	{
		theApp.changeViewport(MeshWnd, "MeshWnd");
	}
	else if (strTitle == "GuiWnd - MiniOne")
	{
		theApp.changeViewport(GuiWnd, "GuiWnd");
	}
	
	CMDIFrameWndEx::OnSetText(strTitle.c_str());
	Invalidate();
	return 0;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);
	
	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
		// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndClassView);

	m_wndAsset.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndAsset);

	m_wndTools.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTools);

	m_wndSceneView.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndSceneView);

	m_wndGuiView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndGuiView);
	
	CDockablePane* pTabbedBar = nullptr;
	//m_wndGuiView.AttachToTabWnd(&m_wndSceneView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);


	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	m_strTitle = TEXT("MiniOne");
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}


	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}


	// Create asset window
	CString strAssetWnd;
	bNameValid = strAssetWnd.LoadString(IDS_ASSET_WND);
	ASSERT(bNameValid);
	if (!m_wndAsset.Create(strAssetWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_ASSETWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create tools window
	CString strToolsWnd;
	bNameValid = strToolsWnd.LoadString(IDS_TOOLS_WND);
	ASSERT(bNameValid);
	if (!m_wndTools.Create(strToolsWnd, this, CRect(0, 200, 200, 200), TRUE, ID_VIEW_TOOLSWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Asset window\n");
		return FALSE; // failed to create
	}

	// Create scene view
	CString strSceneView;
	bNameValid = strSceneView.LoadString(IDS_SCENE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSceneView.Create(strSceneView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Scene View window\n");
		return FALSE; // failed to create
	}

	// Create gui view
	CString strGuiView;
	bNameValid = strGuiView.LoadString(IDS_GUI_VIEW);
	ASSERT(bNameValid);
	if (!m_wndGuiView.Create(strGuiView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Scene View window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

CSceneView* CMainFrame::getSceneView()
{
	return &m_wndSceneView;
}

CGuiView* CMainFrame::getGuiView()
{
	return &m_wndGuiView;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hAssetBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndAsset.SetIcon(hAssetBarIcon, FALSE);

	HICON hToolsBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTools.SetIcon(hToolsBarIcon, FALSE);

	HICON hSceneViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_SCENE_VIEW_HC : IDI_SCENE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSceneView.SetIcon(hSceneViewIcon, FALSE);

	HICON hGuiViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_SCENE_VIEW_HC : IDI_SCENE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndGuiView.SetIcon(hGuiViewIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}
	/*
	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	*/
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	/*
	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}
	*/

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	static MESSAGE msg;
	CView* pView = theApp.GetActiveView();
	if( pView == nullptr )
	{
		return FALSE;
	}
	msg.bProcess = false;
	msg.uMessageType = pMsg->message;

	CRect rect;
	pView->GetClientRect(&rect);

	CPoint point = { pMsg->pt.x, pMsg->pt.y };
	pView->ScreenToClient(&point);

	msg.x = (float)point.x * (float)GEngine->getDeviceWidth() / rect.Width();
	msg.y = (float)point.y * (float)GEngine->getDeviceHeight() / rect.Height();
	GEngine->ProcessMsg(msg);

	switch (pMsg->message)
	{
	case CUSTOMMSGClassView:
	{

	}
	break;
	case CUSTOMMSGAssetView:
	{

	}
	break;
	case CUSTOMMSGFileView:
	{

	}
	break;
	case WM_LBUTTONDOWN:
	{
#if 1

#else
		if (nullptr == m_pStatic)
		{
			m_pStatic = new CStatic;
			m_pStatic->Create(TEXT(""), SS_CENTER | SS_CENTERIMAGE | WS_CLIPCHILDREN | WS_CHILD | WS_VISIBLE, CRect(0, 0, 1, 1), this, 1314);
		}
		else
		{
			m_pStatic->ShowWindow(TRUE);
		}
#endif
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (nullptr != m_pStatic)
		{
			CRect rect;
			GetWindowRect(&rect);
			ScreenToClient(&rect);

			CPoint point;
			GetCursorPos(&point);

			CRect pRect;
			GetClientRect(&pRect);
			ScreenToClient(&point);

			m_pStatic->MoveWindow(point.x - 50, point.y - 15, 100, 30);
			m_pStatic->SetWindowText(TEXT("111"));
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		if ((short)HIWORD(pMsg->wParam) > 0)
		{

		}
		else
		{

		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (nullptr != m_pStatic)
		{
			m_pStatic->ShowWindow(FALSE);
		}
	}
	break;
	default:
		break;
	}
	return FALSE;
}