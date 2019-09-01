
// MiniOneEditor.h : main header file for the MiniOneEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MiniOne.h"

// CMiniOneEditorApp:
// See MiniOneEditor.cpp for the implementation of this class
//

class CMiniOneEditorApp : public CWinAppEx
{
public:
	CMiniOneEditorApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void clickEvent(UINT32 Msg);
	void setClickItem(CString itemName);
	void insertEntityToSceneView(CEntity* pEntity);
	void createScene(const std::string& sceneName);

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	CEngine*	mpEngine;
	virtual void OnClosingMainFrame(CFrameImpl* pFrameImpl);
};

extern CMiniOneEditorApp theApp;
