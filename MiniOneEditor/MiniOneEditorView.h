
// MiniOneEditorView.h : interface of the CMiniOneEditorView class
//

#pragma once


class CMiniOneEditorView : public CView
{
protected: // create from serialization only
	CMiniOneEditorView() noexcept;
	DECLARE_DYNCREATE(CMiniOneEditorView)

// Attributes
public:
	CMiniOneEditorDoc* GetDocument() const;
	static CMiniOneEditorView* gpEditorView;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMiniOneEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBaseTriangle();
	afx_msg void OnTexture();
	afx_msg void OnDiffuse();
	afx_msg void OnCubeMesh();
	afx_msg void OnGui();
};

#ifndef _DEBUG  // debug version in MiniOneEditorView.cpp
inline CMiniOneEditorDoc* CMiniOneEditorView::GetDocument() const
   { return reinterpret_cast<CMiniOneEditorDoc*>(m_pDocument); }
#endif

