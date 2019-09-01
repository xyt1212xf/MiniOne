
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ClassView.h"
#include "PropertiesWnd.h"
#include "AssetWnd.h"
#include "ToolsWnd.h"
#include "SceneView.h"
#include "GuiView.h"

#define CUSTOMMSGClassView WM_USER + 1
#define CUSTOMMSGAssetView WM_USER + 2
#define CUSTOMMSGFileView WM_USER + 3


class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSceneView* getSceneView();
	CGuiView*	getGuiView();
	void setClickItem(CString itemName);
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CClassView        m_wndClassView;
	CPropertiesWnd    m_wndProperties;

	CAssetWnd		  m_wndAsset;
	CToolsWnd		m_wndTools;
	CSceneView		m_wndSceneView;
	CGuiView		m_wndGuiView;
	CString			mItemName;
	CStatic*		m_pStatic;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


