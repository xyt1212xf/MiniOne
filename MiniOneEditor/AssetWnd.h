
#pragma once
#include <list>
#include <map>
#include "ButtonEx.h"


class CAssetWnd : public CDockablePane
{
// Construction
public:
	CAssetWnd() noexcept;

	void AdjustLayout();


// Implementation
public:
	virtual ~CAssetWnd();
	virtual BOOL OnShowControlBarMenu(CPoint point) { return TRUE; }

protected:
	afx_msg void OnNMClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblickListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnClickedAddNewBtn();
	afx_msg void OnUpdateAddNewBtn(CCmdUI* pCmdUI);

	afx_msg void OnClickedSaveBtn();
	afx_msg void OnUpdateSaveBtn(CCmdUI* pCmdUI);

	afx_msg void OnClickedFrameLineBtn();
	afx_msg void OnUpdateFrameLineBtn(CCmdUI* pCmdUI);

	afx_msg void OnFindAssetFolder();
	afx_msg void OnFiltersAsset();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

	void InitTreeList();
	int	 ResetResourceList(CString path, CString subString);
	void EnumCurrentFolderContent(CString path);
	void addTreeList();
	void drawFrameLine();
	void insertAssetTo(CString name, CString exterName);

protected:
	CFont m_fntPropList;
	CEdit		m_wndObjectEdit;
	CEdit		m_findResourceEdit;
	CImageList	m_AssetViewImages;
	CTreeCtrl	m_wndTreeCtrl;
	CListCtrl	m_wndListCtrl;
	CButton		m_AddNewItem;
	CButton		m_SaveItem;
	CButtonEx	mFrameLine;

	CStatic		mFrameLineHorizontal;
	CStatic		mBackColor;
	CStatic		mAssetPath;
	int m_nButtonHeight;
	int	m_nTreeListWidth;
	CPoint mMousePt;
	bool mbHaveAdd;
	bool mbUpdateArea;
	CImageList    mAssetImageList;
	std::list<CString>	mAddTreeItemName;
	std::map<CString, INT32>	mImageMaps;
	CString		mFilterString;
	CString		mSearchPath;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

extern CAssetWnd* gAssetWnd;