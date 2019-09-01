
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Editor.h"
#include "SceneView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
CPropertiesWnd* gPropertiesWnd = nullptr;
CPropertiesWnd::CPropertiesWnd() noexcept
{
	mpEntity = nullptr;
	m_nComboHeight = 0;
	gPropertiesWnd = this;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers
LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;	
	auto item = mPropertyMap.find(pProp);
	Property* pProperty = (item->second).pProperty;
	if (pProperty->dataType >= _FLOAT && pProperty->dataType <= _COLOR24)
	{
		COleVariant value = pProp->GetValue(); //改变之后的值
		memcpy((item->second).pMemory, &value.fltVal, sizeof(float));
		mpEntity->updateUnitProperty(pProperty);
	}
	mpEntity->dirtyProperty();
	return 0;
}


void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	mActorName.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), 26, SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + 25, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::setChooseEntity(CEntity* pEntity)
{
	m_wndPropList.RemoveAll();
	m_wndPropList.RedrawWindow();
	mPropertyMap.clear();

	mpEntity = pEntity;
	if (nullptr == mpEntity)
	{
		mActorName.SetWindowText("Please select actor object, level is not allow editor!");
		m_wndPropList.ShowWindow(FALSE);
	}
	else
	{
		mActorName.SetWindowText(pEntity->getName().c_str());
		m_wndPropList.ShowWindow(TRUE);
		auto units = pEntity->getUnits();
		for( auto item : units )
		{	
			Property* pProperty = item->getPropertyFirst();
			CMFCPropertyGridProperty* pGroup = nullptr;
			while (pProperty != nullptr)
			{
				pGroup = insertProperty(pProperty, pGroup);
				pProperty = item->getPropertyNext();
			}
		}	
	}	
}

CMFCPropertyGridProperty* CPropertiesWnd::insertProperty(Property* pProperty, CMFCPropertyGridProperty* pgroup)
{
	if (pProperty->nFlag & PropertyFlag::GroundName)
	{
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(pProperty->strName.c_str());
		m_wndPropList.AddProperty(pGroup);
		return pGroup;	
	}
	else
	{
		float* pData = static_cast<float*>(pProperty->pMemory);
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(pProperty->strName.c_str(), 0, TRUE);
		pgroup->AddSubItem(pSubItem);

		switch (pProperty->dataType)
		{
		case _FLOAT:
		{
			AddSubProperty<float>(pSubItem, pProperty, &pData[0], pProperty->strName.c_str(), pProperty->strName.c_str());
		}break;
		case _FLOAT4:
		{
			float* pData = static_cast<float*>(pProperty->pMemory);
			AddSubProperty<float>(pSubItem, pProperty, &pData[3], "W", "W axis");
		}
		case _FLOAT3:
		{
			float* pData = static_cast<float*>(pProperty->pMemory);
			AddSubProperty<float>(pSubItem, pProperty, &pData[2], "Z", "Z axis");
		}
		case _FLOAT2:
		{
			float* pData = static_cast<float*>(pProperty->pMemory);
			AddSubProperty<float>(pSubItem, pProperty, &pData[0], "X", "X axis");
			AddSubProperty<float>(pSubItem, pProperty, &pData[1], "Y", "Y axis");
		}break;
		case _COLOR32:
		{
			float* pData = static_cast<float*>(pProperty->pMemory);
			AddSubProperty<float>(pSubItem, pProperty, &pData[3], "A", "Transparent Value");
		}
		case _COLOR24:
		{
			float* pData = static_cast<float*>(pProperty->pMemory);
			AddSubProperty<float>(pSubItem, pProperty, &pData[0], "R", "Red Value");
			AddSubProperty<float>(pSubItem, pProperty, &pData[1], "G", "Green Value");
			AddSubProperty<float>(pSubItem, pProperty, &pData[2], "B", "Blue Value");
		}break;
		default:
			delete pSubItem;
			return pgroup;
		}
		return pgroup;
	}
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	m_nComboHeight = 28;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();
	
	mActorName.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_PROPERTIES_ACTORNAME);
	
	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t) false, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);
	m_wndPropList.AddProperty(pGroup3);
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), nullptr, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));



	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));
	m_wndPropList.AddProperty(pGroup4);

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"), 0, TRUE);
	pGroup4->AddSubItem(pGroup41);
	
	pGroup41->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) 1.30, _T("")));
	pGroup41->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)22.3, _T("This is a description")));
	pGroup41->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)33, _T("This is a description")));

	pGroup4->Expand(FALSE);

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}


void CPropertiesWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//block message
}




void CPropertiesWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDockablePane::OnPaint() for painting messages
	CRect rc;
	GetClientRect(rc);
	CBrush   brush;

	brush.CreateSolidBrush(RGB(255, 255, 255));
	dc.FillRect(&rc, &brush);
}
