
// Editor.h : main header file for the Editor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MiniOne.h"
#include "childFrm.h"

// CEditorApp:
// See Editor.cpp for the implementation of this class
//

class CEditorApp : public CWinAppEx
{
public:
	CEditorApp() noexcept;

private:
	CEngine*	mpEngine;
	std::map<MidViewType, CView*>	mMidViewWnds;
	CMultiDocTemplate* mpDocTemplate;
	bool				mChangeSwapping = true;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CView*		GetActiveView();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	CView*	mpActiveView;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
	void clickEvent(UINT32 Msg);
	void setClickItem(CString itemName);
	void insertEntityToSceneView(CEntity* pEntity);
	void createScene(const std::string& sceneName);
	void changeViewport(MidViewType type, std::string viewportName);
	void removeViewport(MidViewType type);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnViewportsSceneWnd();
	afx_msg void OnViewportsGuiWnd();
	afx_msg void OnViewportsMaterialWnd();
	afx_msg void OnViewportsMeshWnd();
};

extern CEditorApp theApp;
