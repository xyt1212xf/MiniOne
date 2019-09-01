
// EditorView.cpp : implementation of the CEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include "EditorDoc.h"
#include "EditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BASE_TRIANGLE, &CEditorView::OnBaseTriangle)
	ON_COMMAND(ID_TEXTURE, &CEditorView::OnTexture)
	ON_COMMAND(ID_DIFFUSE, &CEditorView::OnDiffuse)
	ON_COMMAND(ID_CUBEMESH, &CEditorView::OnCubeMesh)
	ON_COMMAND(ID_CREATEGUI, &CEditorView::OnGui)
END_MESSAGE_MAP()

// CEditorView construction/destruction

CEditorView::CEditorView() noexcept
{
	// TODO: add construction code here

}

CEditorView::~CEditorView()
{
	if (GEngine != nullptr)
	{
		CString strTitle = GetDocument()->GetTitle();
		if (strTitle == "SceneWnd")
		{
			theApp.removeViewport(SceneWnd);
		}
		else if (strTitle == "MaterialWnd")
		{
			theApp.removeViewport(MaterialWnd);
		}
		else if (strTitle == "MeshWnd")
		{
			theApp.removeViewport(MeshWnd);
		}
		else if (strTitle == "GuiWnd")
		{
			theApp.removeViewport(GuiWnd);
		}
		theApp.changeViewport(SceneWnd, "SceneWnd");
	}
}

BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEditorView drawing

void CEditorView::OnDraw(CDC* /*pDC*/)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CEditorView printing


void CEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifdef EDITOR
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_EDIT_MENU, point.x, point.y, this, TRUE);
#else
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
#endif
}


// CEditorView diagnostics

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CEditorView message handlers


void CEditorView::OnBaseTriangle()
{
	// TODO: Add your command handler code here
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(Triangle);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CEditorView::OnTexture()
{
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(TriangleTexture);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CEditorView::OnDiffuse()
{
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(Diffuse);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CEditorView::OnCubeMesh()
{
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(CubeMesh);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CEditorView::OnGui()
{
	GEngine->stopRender();
	CEntity* pGui = GEngine->createActor(Gui);
	theApp.insertEntityToSceneView(pGui);
	SafeRelease(pGui);
	GEngine->recoverRender();
}