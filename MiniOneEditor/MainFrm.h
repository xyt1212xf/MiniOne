
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "SceneView.h"
#include "ClassView.h"
#include "PropertiesWnd.h"
#include "AssetWnd.h"
#include "ToolsWnd.h"

#define CUSTOMMSGClassView WM_USER + 1
#define CUSTOMMSGAssetView WM_USER + 2
#define CUSTOMMSGFileView WM_USER + 3

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// Implementation
public:
	virtual ~CMainFrame();
	void setClickItem(CString itemName);
	CSceneView* getSceneView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CSceneView         m_wndSceneView;
	CClassView        m_wndClassView;
	CPropertiesWnd    m_wndProperties;
	CAssetWnd		  m_wndAsset;
	CToolsWnd			m_wndTools;
	CStatic*		m_pStatic;
	CString			mItemName;
	CView*			mpActiveView;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


