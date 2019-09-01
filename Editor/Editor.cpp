
// Editor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Editor.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "EditorDoc.h"
#include "EditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorApp

BEGIN_MESSAGE_MAP(CEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

	ON_COMMAND(ID_VIEWPORTS_SCENE, &CEditorApp::OnViewportsSceneWnd)
	ON_COMMAND(ID_VIEWPORTS_GUI, &CEditorApp::OnViewportsGuiWnd)
	ON_COMMAND(ID_VIEWPORTS_MATERIAL, &CEditorApp::OnViewportsMaterialWnd)
	ON_COMMAND(ID_VIEWPORTS_MESH, &CEditorApp::OnViewportsMeshWnd)

	ON_COMMAND(ID_VIEWPORTS_SCENE32777, &CEditorApp::OnViewportsSceneWnd)
	ON_COMMAND(ID_VIEWPORTS_GUI32778, &CEditorApp::OnViewportsGuiWnd)
	ON_COMMAND(ID_VIEWPORTS_MATERIALWND, &CEditorApp::OnViewportsMaterialWnd)
	ON_COMMAND(ID_VIEWPORTS_MESHWND, &CEditorApp::OnViewportsMeshWnd)
END_MESSAGE_MAP()


// CEditorApp construction

CEditorApp::CEditorApp() noexcept
{
	mpEngine = nullptr;
	m_bHiColorIcons = TRUE;
	mpDocTemplate = nullptr;
	mpActiveView = nullptr;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Editor.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CEditorApp object

CEditorApp theApp;


// CEditorApp initialization

BOOL CEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_EditorTYPE,
		RUNTIME_CLASS(CEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	mpDocTemplate = pDocTemplate;
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	CMDIFrameWnd *pFrame = dynamic_cast<CMDIFrameWnd*>(m_pMainWnd);
	CMDIChildWnd *pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	CView *pView = (CView *)pChild->GetActiveView();
	CString sx = pChild->GetActiveDocument()->GetTitle();
	pChild->GetActiveDocument()->SetTitle("SceneWnd");
	mMidViewWnds[SceneWnd] = pView;
	mpActiveView = pView;
	CSceneView* pSceneView = dynamic_cast<CMainFrame*>(m_pMainWnd)->getSceneView();
	if (pSceneView != nullptr)
	{
		pSceneView->insertItem(EntityType::Level, "defalultScene", nullptr);
	}

	mpEngine = createEngine(pView->GetSafeHwnd());
	if (nullptr != mpEngine)
	{
		mpEngine->onStart();
		mpEngine->onRun();
	}
	createScene("defaultScene");
	return TRUE;
}
//#include "Terrain.h"

void CEditorApp::createScene(const std::string& sceneName)
{
	mpEngine->createScene(sceneName);
	mpEngine->getActiveCamera();
	
	GEngine->stopRender();
	mpEngine->createTerrain("Resource/Texture/heightmap.bmp");
	CEntity* pTerrain = mpEngine->getTerrainActor();
	theApp.insertEntityToSceneView(pTerrain);
	GEngine->recoverRender();
	SafeRelease(pTerrain);

	//theApp.insertEntityToSceneView(pEntity);
}

int CEditorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

CView* CEditorApp::GetActiveView()
{
	return mpActiveView;
}

// CEditorApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEditorApp customization load/save methods

void CEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
#ifdef EDITOR 
	GetContextMenuManager()->AddMenu(strName, IDR_EDIT_MENU);
#else
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
#endif

	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CEditorApp::LoadCustomState()
{
}

void CEditorApp::SaveCustomState()
{
}


void CEditorApp::OnClosingMainFrame(CFrameImpl* pFrameImpl)
{
	// TODO: Add your specialized code here and/or call the base class
	if (nullptr != mpEngine)
	{
		mpEngine->onShutDown();
		destoryEngine();
	}
	return CWinAppEx::OnClosingMainFrame(pFrameImpl);
}

// CEditorApp message handlers



void CEditorApp::setClickItem(CString itemName)
{
	dynamic_cast<CMainFrame*>(m_pMainWnd)->setClickItem(itemName);
}

void CEditorApp::insertEntityToSceneView(CEntity* pEntity)
{
	CSceneView* pSceneView = dynamic_cast<CMainFrame*>(m_pMainWnd)->getSceneView();
	pSceneView->insertItem(pEntity);
	CScene* pScene = GEngine->getCurrentScene();
	if (pScene != nullptr)
	{
		GEngine->addActorToScene(pEntity);
	}
	else
	{
#ifdef WIN32
		MessageBox(NULL, "scene isn't exist, must be to create before addEntity", "error", 0);
#endif
	}
}

void CEditorApp::clickEvent(UINT32 Msg)
{

}

void CEditorApp::changeViewport(MidViewType type, std::string viewportName)
{
	if (GEngine != nullptr && mChangeSwapping)
	{
		mChangeSwapping = false;
		auto item = mMidViewWnds.find(type);
		GEngine->stopRender();
		if (item == mMidViewWnds.end())
		{
			CDocument* pDocument = mpDocTemplate->OpenDocumentFile(nullptr);
			pDocument->SetTitle(viewportName.c_str());
			POSITION pos = pDocument->GetFirstViewPosition();
			CView* pView = pDocument->GetNextView(pos);
			mMidViewWnds[type] = pView;
			mpEngine->createRenderWindow(pView->GetSafeHwnd());
			mpEngine->changeRenderWindow(pView->GetSafeHwnd());
			mpActiveView = pView;
		}
		else
		{
			mpEngine->changeRenderWindow((item->second)->GetSafeHwnd());
			mpActiveView = item->second;
		}
		GEngine->recoverRender();
		
		mChangeSwapping = true;
		switch (type)
		{
		case SceneWnd:
			mpEngine->activeRenderer(DeferredRenderer | FrontRenderer | ShadowRenderer | PostRenderer | GuiRenderer);
			mpEngine->disableRendererEditor(GuiRenderer);
		case MaterialWnd:
		case MeshWnd:
			break;
		case GuiWnd:
			mpEngine->activeRenderer(GuiRenderer);
			mpEngine->activeRendererEditor(GuiRenderer);
			break;
		default:
			break;
		}
	}
}


void CEditorApp::removeViewport(MidViewType type)
{
	auto item = mMidViewWnds.find(type);
	if( item != mMidViewWnds.end() )
	{
		mMidViewWnds.erase(item);
	}
}


void CEditorApp::OnViewportsSceneWnd()
{
	// TODO: Add your command handler code here
	changeViewport(SceneWnd, "SceneWnd");
}


void CEditorApp::OnViewportsGuiWnd()
{
	// TODO: Add your command handler code here
	changeViewport(GuiWnd, "GuiWnd");
}

void CEditorApp::OnViewportsMaterialWnd()
{
	// TODO: Add your command handler code here
	changeViewport(MaterialWnd, "MaterialWnd");
}

void CEditorApp::OnViewportsMeshWnd()
{
	// TODO: Add your command handler code here
	changeViewport(MeshWnd, "MeshWnd");
}

