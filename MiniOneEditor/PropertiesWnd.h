
#pragma once
#include "Engine.h"
struct PropertySubItem
{
	Property* pProperty;
	void*	  pMemory;
};

class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd() noexcept;
	virtual ~CPropertiesWnd();
	void AdjustLayout();
	void setChooseEntity(CEntity* pEntity);
	virtual BOOL OnShowControlBarMenu(CPoint point) { return TRUE; }

	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_wndPropList;
	CStatic		mBackColor;
	CEdit		mActorName;
	CEntity*	mpEntity;
	std::map<CMFCPropertyGridProperty*, PropertySubItem> mPropertyMap;

// Implementation
protected:
	CMFCPropertyGridProperty* insertProperty(Property* pProperty, CMFCPropertyGridProperty* pGroup);

	template<typename T>
	void AddSubProperty( CMFCPropertyGridProperty* pSubItem, Property* pProperty, T* pDataAddress, std::string strName, std::string strDescription)
	{
		PropertySubItem subItem;
		subItem.pProperty = pProperty;
		subItem.pMemory = pDataAddress;
		T data = *pDataAddress;
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(strName.c_str(), (_variant_t)data, strDescription.c_str());	
		pSubItem->AddSubItem(pProp);
		mPropertyMap[pProp] = subItem;
	}

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
	int m_nComboHeight;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};

extern CPropertiesWnd* gPropertiesWnd;