
#include "stdafx.h"
#include "AssetWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "MiniOneEditor.h"
#include "Foundation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
int frameLineHeight = 8;
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
CAssetWnd* gAssetWnd = nullptr;
CAssetWnd::CAssetWnd() noexcept
{
	gAssetWnd = this;
	m_nTreeListWidth = 300;
	mbUpdateArea = false;
}

CAssetWnd::~CAssetWnd()
{
}

BEGIN_MESSAGE_MAP(CAssetWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	
	ON_COMMAND(IDC_ASSET_SAVEBTN, OnClickedSaveBtn)
	ON_UPDATE_COMMAND_UI(IDC_ASSET_SAVEBTN, OnUpdateSaveBtn)

	ON_COMMAND(IDC_ASSET_ADDNEWBTN, OnClickedAddNewBtn)
	ON_UPDATE_COMMAND_UI(IDC_ASSET_ADDNEWBTN, OnUpdateAddNewBtn)

	ON_COMMAND(IDC_ASSET_FRAMELINE, OnClickedFrameLineBtn)
	ON_UPDATE_COMMAND_UI(IDC_ASSET_FRAMELINE, OnUpdateFrameLineBtn)

	ON_EN_CHANGE(IDC_ASSET_FINDFOLDER, OnFindAssetFolder)
	ON_EN_CHANGE(IDC_ASSET_FINDEDIT, OnFiltersAsset)
	ON_NOTIFY(NM_CLICK, IDC_ASSET_TREECTRL, OnNMClickTreeCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_ASSET_LISTCTRL, OnDblickListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers
void CAssetWnd::OnNMClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	UINT uFlag;
	HTREEITEM hTree;	
	GetCursorPos(&point);
	m_wndTreeCtrl.ScreenToClient(&point);
	hTree = m_wndTreeCtrl.HitTest(point, &uFlag);
	CString name = TEXT("");
	if (hTree && (TVHT_ONITEM & uFlag))
	{
		while (hTree != nullptr && hTree != m_wndTreeCtrl.GetRootItem())
		{
			if (name.GetLength() > 0)
			{
				name = m_wndTreeCtrl.GetItemText(hTree) + "/" + name;
			}
			else
			{
				name = m_wndTreeCtrl.GetItemText(hTree);
			}
			hTree = m_wndTreeCtrl.GetParentItem(hTree);
		}
		mSearchPath = name;
		EnumCurrentFolderContent("resource/" + mSearchPath);
		mAssetPath.SetWindowText("AssetPaht: Content/" + mSearchPath);
	}

	*pResult = 0;
}

void CAssetWnd::OnDblickListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	POSITION pos = m_wndListCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_wndListCtrl.GetNextSelectedItem(pos);
		CString name = m_wndListCtrl.GetItemText(nItem, 0);
		std::string exterName = Foundation::getExterName(name.GetString());
		if (exterName.length() == 0)
		{
			int length = mSearchPath.GetLength();
			if (length > 0 && mSearchPath[length-1] != '/')
			{
				mSearchPath += "/";
			}
			mSearchPath = mSearchPath + name;
			EnumCurrentFolderContent("resource/" + mSearchPath);
			mAssetPath.SetWindowText("AssetPaht: Content/" + mSearchPath);
		}
	}
	else
	{
		int nStart = mSearchPath.ReverseFind('/');
		if (-1 != nStart)
		{
			CString strResult = mSearchPath.Left(nStart);
			mSearchPath = strResult;
		}
		else
		{
			mSearchPath = "";
		}
		EnumCurrentFolderContent("resource/" + mSearchPath);
		mAssetPath.SetWindowText("AssetPaht: Content/" + mSearchPath);
	}
}

void CAssetWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	mBackColor.SetWindowPos(nullptr, rectClient.left, rectClient.top + frameLineHeight, rectClient.Width(), m_nButtonHeight, SWP_NOZORDER);

	mAssetPath.SetWindowPos(nullptr, m_nTreeListWidth+50, 0, rectClient.Width(), m_nButtonHeight, SWP_NOZORDER);
	
	mFrameLine.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth, rectClient.top + frameLineHeight, frameLineHeight, rectClient.Height(), SWP_NOZORDER);

	m_AddNewItem.SetWindowPos(nullptr, 0, rectClient.top + frameLineHeight, 80, m_nButtonHeight, SWP_NOZORDER);
	
	m_SaveItem.SetWindowPos(nullptr, 20 + 80, rectClient.top + frameLineHeight, 80, m_nButtonHeight, SWP_NOZORDER);
	
	mFrameLineHorizontal.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), frameLineHeight, SWP_NOZORDER);
	
	mFrameLine.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth, rectClient.top + frameLineHeight + m_nButtonHeight, frameLineHeight, rectClient.Height() - (m_nButtonHeight)-frameLineHeight, SWP_NOZORDER);

	m_wndObjectEdit.SetWindowPos(nullptr, rectClient.left, rectClient.top + frameLineHeight + m_nButtonHeight, m_nTreeListWidth, m_nButtonHeight, SWP_NOZORDER);

	m_wndTreeCtrl.SetWindowPos(nullptr, rectClient.left, rectClient.top + frameLineHeight + m_nButtonHeight * 2, m_nTreeListWidth, rectClient.Height() - m_nButtonHeight * 2 - frameLineHeight, SWP_NOZORDER);

	m_findResourceEdit.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth + frameLineHeight, rectClient.top + frameLineHeight + m_nButtonHeight, rectClient.Width() - m_nTreeListWidth - frameLineHeight, m_nButtonHeight, SWP_NOACTIVATE);

	m_wndListCtrl.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth + frameLineHeight, rectClient.top + frameLineHeight + m_nButtonHeight + m_nButtonHeight, rectClient.Width() - m_nTreeListWidth - frameLineHeight, rectClient.Height() - (m_nButtonHeight * 2)-frameLineHeight, SWP_NOACTIVATE);
}

int CAssetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	mFilterString = "";
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	mBackColor.Create(TEXT(""), WS_CHILD | WS_VISIBLE | SS_CENTER, rectDummy, this, IDC_ASSET_BACKCOLOR);

	mAssetPath.Create(TEXT("AssetPaht: Content/"), WS_CHILD | WS_VISIBLE | SS_LEFT, rectDummy, &mBackColor, IDC_ASSET_BACKCOLOR);

	//CWnd* pParent = mBackColor.GetWindow();
	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	m_wndObjectEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_ASSET_FINDFOLDER);

	m_findResourceEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_ASSET_FINDEDIT);

	m_AddNewItem.Create(TEXT("AddNew"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_ASSET_SAVEBTN);

	m_SaveItem.Create(TEXT("Save"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_ASSET_SAVEBTN);

	mFrameLine.Create(TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_ASSET_FRAMELINE);


	m_nButtonHeight = 28;
	m_AssetViewImages.Create(IDB_SCENE_VIEW, 16, 1, RGB(255, 0, 255));
	if (!m_wndTreeCtrl.Create(TVS_LINESATROOT | TVS_HASBUTTONS | WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_ASSET_TREECTRL))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}
	m_wndTreeCtrl.SetImageList(&m_AssetViewImages, TVSIL_NORMAL);

	m_wndListCtrl.Create( LVS_ICON |  WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_ASSET_LISTCTRL);
	m_wndListCtrl.RedrawWindow();
	mFrameLineHorizontal.Create(TEXT(""), WS_CHILD | WS_VISIBLE | SS_CENTER, rectDummy, this, IDC_ASSET_FRAMELINE_H);
	
	mAssetImageList.Create(64, 64, ILC_COLORDDB | ILC_COLOR24, 100, 1);     //wide of picture is 64,height is 64
	m_wndListCtrl.SetImageList(&mAssetImageList, LVSIL_NORMAL);     //CImageList associates CListCtrl

	InitTreeList();
	AdjustLayout();
	return 0;
}

void CAssetWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CAssetWnd::OnUpdateSaveBtn(CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}

void CAssetWnd::OnUpdateFrameLineBtn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CAssetWnd::OnUpdateAddNewBtn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CAssetWnd::OnClickedFrameLineBtn()
{
	mbUpdateArea = false;
}


void CAssetWnd::OnClickedSaveBtn()
{

}

void CAssetWnd::OnClickedAddNewBtn()
{

}

void CAssetWnd::InitTreeList()
{
	HTREEITEM hTreeRoot = m_wndTreeCtrl.InsertItem(TEXT("Content"));
	m_wndTreeCtrl.GetRootItem();
	mbHaveAdd = true;
	ResetResourceList(TEXT("resource"), TEXT(""));
	mSearchPath = "";
	EnumCurrentFolderContent("resource");
}

void CAssetWnd::OnFindAssetFolder()
{
	CString strContent;
	m_wndObjectEdit.GetWindowText(strContent);
	m_wndTreeCtrl.DeleteAllItems();
	HTREEITEM hTreeRoot = m_wndTreeCtrl.InsertItem(TEXT("Content"));
	mbHaveAdd = false;
	ResetResourceList(TEXT("resource"), strContent);
}

void CAssetWnd::OnFiltersAsset()
{
	CString strContent;
	m_findResourceEdit.GetWindowText(strContent);
	mFilterString = strContent;
	EnumCurrentFolderContent("resource/" + mSearchPath);
}

int CAssetWnd::ResetResourceList(CString path, CString subString )
{
	CFileFind find;
	BOOL IsFind = find.FindFile(path + "/*.*");

	while (IsFind)
	{
		IsFind = find.FindNextFile();
		CString filename = find.GetFilePath();//获取文件的路径，可能是文件夹，可能是文件 
		if (find.IsDots())
		{
			continue;
		}
		else
		{
			if (find.IsDirectory() )
			{
				CString filename = find.GetFileName();
				if (-1 != filename.Find(subString, 0))
				{
					mbHaveAdd = true;
				}
				CString fullname = path + "/" + filename;
				mAddTreeItemName.push_back(filename);
				ResetResourceList(fullname, subString);
				addTreeList();
			}
		}
	}
	return 0;
}

void CAssetWnd::EnumCurrentFolderContent(CString path)
{
	CFileFind find;
	BOOL IsFind = find.FindFile(path + "/*.*");
	m_wndListCtrl.DeleteAllItems();
	while (IsFind)
	{
		IsFind = find.FindNextFile();
		CString filename = find.GetFilePath();//获取文件的路径，可能是文件夹，可能是文件 
		if (find.IsDots())
		{
			continue;
		}
		else
		{
			CString& filename = find.GetFileName();
			if (find.IsDirectory())
			{
				insertAssetTo(filename, "");
			}
			else if (find.IsArchived() || find.IsNormal())
			{
				const std::string& exterName = Foundation::getExterName(filename.GetString());
				if (mFilterString.GetLength() > 0)
				{
					if (mFilterString != exterName.c_str())
					{
						insertAssetTo(filename, exterName.c_str());
					}
				}
				else
				{
					insertAssetTo(filename, exterName.c_str());
				}
			}
		}
	}
}

void CAssetWnd::addTreeList()
{
	EnableWindow(TRUE);
	m_SaveItem.EnableWindow(TRUE);
	if (mbHaveAdd)
	{
		HTREEITEM treeitem = m_wndTreeCtrl.GetRootItem();
		for (auto item : mAddTreeItemName)
		{
			treeitem = m_wndTreeCtrl.InsertItem(item, treeitem);
		}
	}
	mAddTreeItemName.clear();
	mbHaveAdd = false;
}


void CAssetWnd::drawFrameLine()
{
	CRect rectClient;
	GetClientRect(rectClient);
	if (mbUpdateArea)
	{
		CPoint point;
		GetCursorPos(&point);
		m_nTreeListWidth += point.x - mMousePt.x;

		mFrameLine.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth, rectClient.top + frameLineHeight + m_nButtonHeight, frameLineHeight, rectClient.Height() - (m_nButtonHeight)-frameLineHeight, SWP_NOZORDER);

		m_wndObjectEdit.SetWindowPos(nullptr, rectClient.left, rectClient.top + frameLineHeight + m_nButtonHeight, m_nTreeListWidth, m_nButtonHeight, SWP_NOZORDER);

		m_wndTreeCtrl.SetWindowPos(nullptr, rectClient.left, rectClient.top + frameLineHeight + m_nButtonHeight * 2, m_nTreeListWidth, rectClient.Height() - m_nButtonHeight * 2 - frameLineHeight, SWP_NOZORDER);

		m_findResourceEdit.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth + frameLineHeight, rectClient.top + frameLineHeight + m_nButtonHeight, rectClient.Width() - m_nTreeListWidth - frameLineHeight, m_nButtonHeight, SWP_NOACTIVATE);;

		m_wndListCtrl.SetWindowPos(nullptr, rectClient.left + m_nTreeListWidth + frameLineHeight, rectClient.top + frameLineHeight + m_nButtonHeight + m_nButtonHeight, rectClient.Width() - m_nTreeListWidth - frameLineHeight, rectClient.Height() - (m_nButtonHeight * 2)-frameLineHeight, SWP_NOACTIVATE);

		m_wndListCtrl.RedrawWindow();

		mMousePt.x = point.x;
	}
	else
	{
		GetCursorPos(&mMousePt);
		mbUpdateArea = true;
	}	
}

void CAssetWnd::insertAssetTo(CString name, CString exterName)
{
	CString FilePathName = "";   //在资源的IDB_BITMAP1文件就是face.bmp导入的
	if (exterName.GetLength() > 0)
	{
		if (exterName == "m")
		{
			FilePathName = "editor/Mesh.bmp";
		}
		else if (exterName == "fbx")
		{
			FilePathName = "editor/Mesh2.bmp";
		}
		else if (exterName == "dds" || exterName == "png" || exterName == "bmp")
		{
			FilePathName = "editor/Texture.bmp";
		}
		else if (exterName == "act")
		{
			FilePathName = "editor/Actor.bmp";
		}
		else if (exterName == "level")
		{
			FilePathName = "editor/Level.bmp";
		}
		else if (exterName == "vs")
		{
			FilePathName = "editor/ShaderV.bmp";
		}
		else if (exterName == "ps")
		{
			FilePathName = "editor/ShaderP.bmp";
		}
		else if (exterName == "ma")
		{
			FilePathName = "editor/Material.bmp";
		}
		else if (exterName == "gui")
		{
			FilePathName = "editor/Gui.bmp";
		}
		else if (exterName == "lua")
		{
			FilePathName = "editor/Script.bmp";
		}
	}
	else
	{
		FilePathName = "editor/Folder.bmp";
	}
	int index = 0;
	auto item = mImageMaps.find(FilePathName);
	if (item == mImageMaps.end())
	{
		HBITMAP hBitmap;
		CBitmap *pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL, FilePathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		pBitmap->Attach(hBitmap);
		index = mAssetImageList.Add(pBitmap, RGB(0, 0, 0));
		mImageMaps[FilePathName] = index;
		delete pBitmap;
	}
	else
	{
		index = item->second;
	}
	m_wndListCtrl.InsertItem(0, name, index);               
}

void CAssetWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//block  floating
}


void CAssetWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1:
	{
		INT32 nCheckLMouseDown = GetAsyncKeyState(VK_LBUTTON);
		if (nCheckLMouseDown != 0)
		{
			drawFrameLine();
		}
	}
	}
}
