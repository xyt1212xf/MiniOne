
#pragma once
#include <string>
#include <map>
#include "ViewTree.h"
#include "Engine.h"

class CSceneView : public CDockablePane
{
	// Construction
public:
	CSceneView() noexcept;
	virtual ~CSceneView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void insertItem(CEntity* pEntity);
	void insertItem(EntityType nType, const std::string& strContent, CEntity* pEntity);
	CEntity* getChooseEntity();

protected:
	CViewTree m_wndSceneView;
	CImageList m_SceneViewImages;
	std::map<HTREEITEM, CEntity*>	mEntitys;
	CEntity*	mpChooseEntity;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

extern CSceneView* gSceneView;
