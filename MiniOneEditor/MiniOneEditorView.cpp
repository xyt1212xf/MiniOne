
// MiniOneEditorView.cpp : implementation of the CMiniOneEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MiniOneEditor.h"
#endif

#include "MiniOneEditorDoc.h"
#include "MiniOneEditorView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CMiniOneEditorView* CMiniOneEditorView::gpEditorView = nullptr;
// CMiniOneEditorView

IMPLEMENT_DYNCREATE(CMiniOneEditorView, CView)

BEGIN_MESSAGE_MAP(CMiniOneEditorView, CView)
	// Standard printing commands
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BASE_TRIANGLE, &CMiniOneEditorView::OnBaseTriangle)
	ON_COMMAND(ID_TEXTURE, &CMiniOneEditorView::OnTexture)
	ON_COMMAND(ID_DIFFUSE, &CMiniOneEditorView::OnDiffuse)
	ON_COMMAND(ID_CUBEMESH, &CMiniOneEditorView::OnCubeMesh)
	ON_COMMAND(ID_CREATEGUI, &CMiniOneEditorView::OnGui)
END_MESSAGE_MAP()

// CMiniOneEditorView construction/destruction

CMiniOneEditorView::CMiniOneEditorView() noexcept
{
	// TODO: add construction code here
	gpEditorView = this;
}

CMiniOneEditorView::~CMiniOneEditorView()
{
}

BOOL CMiniOneEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMiniOneEditorView drawing

void CMiniOneEditorView::OnDraw(CDC* /*pDC*/)
{
	CMiniOneEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMiniOneEditorView printing


void CMiniOneEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMiniOneEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMiniOneEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMiniOneEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMiniOneEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMiniOneEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifdef EDITOR
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_EDIT_MENU, point.x, point.y, this, TRUE);
#else
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
#endif
}


// CMiniOneEditorView diagnostics

#ifdef _DEBUG
void CMiniOneEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMiniOneEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMiniOneEditorDoc* CMiniOneEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMiniOneEditorDoc)));
	return (CMiniOneEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMiniOneEditorView message handlers


void CMiniOneEditorView::OnBaseTriangle()
{
	// TODO: Add your command handler code here
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(Triangle);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CMiniOneEditorView::OnTexture()
{
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(TriangleTexture);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CMiniOneEditorView::OnDiffuse()
{
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(Diffuse);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CMiniOneEditorView::OnCubeMesh()
{
	GEngine->stopRender();
	CEntity* pEntity = GEngine->createActor(CubeMesh);
	theApp.insertEntityToSceneView(pEntity);
	SafeRelease(pEntity);
	GEngine->recoverRender();
}

void CMiniOneEditorView::OnGui()
{
	GEngine->stopRender();
	CEntity* pGui = GEngine->createActor(Gui);
	theApp.insertEntityToSceneView(pGui);
	SafeRelease(pGui);
	GEngine->recoverRender();
}
