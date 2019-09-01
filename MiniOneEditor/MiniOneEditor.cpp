
// MiniOneEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MiniOneEditor.h"
#include "MainFrm.h"

#include "MiniOneEditorDoc.h"
#include "MiniOneEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniOneEditorApp

BEGIN_MESSAGE_MAP(CMiniOneEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMiniOneEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CMiniOneEditorApp construction

CMiniOneEditorApp::CMiniOneEditorApp() noexcept
{
	mpEngine = nullptr;
	m_bHiColorIcons = TRUE;

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
	SetAppID(_T("MiniOneEditor.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMiniOneEditorApp object

CMiniOneEditorApp theApp;


// CMiniOneEditorApp initialization

BOOL CMiniOneEditorApp::InitInstance()
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

	EnableTaskbarInteraction(FALSE);

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
	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMiniOneEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMiniOneEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->UpdateWindow();
	
	CView* pView =  dynamic_cast<CMainFrame*>(m_pMainWnd)->GetActiveView();
	if (nullptr == pView)
	{
		return FALSE;
	}
	CSceneView* pSceneView = dynamic_cast<CMainFrame*>(m_pMainWnd)->getSceneView();
	if (pSceneView != nullptr)
	{
		pSceneView->insertItem(EntityType::Level, "defalultScene", nullptr);
	}

	mpEngine = createEngine(pView->GetSafeHwnd());
	if( nullptr != mpEngine )
	{
		mpEngine->onStart();
		mpEngine->onRun();
	}
	createScene("defaultScene");
	return TRUE;
}


void CMiniOneEditorApp::createScene(const std::string& sceneName)
{
	mpEngine->createScene(sceneName);
	CEntity* pEntity = mpEngine->getActiveCamera();
	//theApp.insertEntityToSceneView(pEntity);
}

int CMiniOneEditorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

void CMiniOneEditorApp::clickEvent(UINT32 Msg)
{

}

void CMiniOneEditorApp::setClickItem(CString itemName)
{
	dynamic_cast<CMainFrame*>(m_pMainWnd)->setClickItem(itemName);
}

void CMiniOneEditorApp::insertEntityToSceneView(CEntity* pEntity)
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


// CMiniOneEditorApp message handlers


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
void CMiniOneEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMiniOneEditorApp customization load/save methods

void CMiniOneEditorApp::PreLoadState()
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

void CMiniOneEditorApp::LoadCustomState()
{
}

void CMiniOneEditorApp::SaveCustomState()
{
}

// CMiniOneEditorApp message handlers

void CMiniOneEditorApp::OnClosingMainFrame(CFrameImpl* pFrameImpl)
{
	// TODO: Add your specialized code here and/or call the base class
	if (nullptr != mpEngine)
	{
		mpEngine->onShutDown();
		destoryEngine();
	}
	return CWinAppEx::OnClosingMainFrame(pFrameImpl);
}



