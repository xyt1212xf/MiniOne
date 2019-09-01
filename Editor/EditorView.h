
// EditorView.h : interface of the CEditorView class
//

#pragma once


class CEditorView : public CView
{
protected: // create from serialization only
	CEditorView() noexcept;
	DECLARE_DYNCREATE(CEditorView)

// Attributes
public:
	CEditorDoc* GetDocument() const;

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
	virtual ~CEditorView();
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
	afx_msg void OnBaseTriangle();
	afx_msg void OnTexture();
	afx_msg void OnDiffuse();
	afx_msg void OnCubeMesh();
	afx_msg void OnGui();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EditorView.cpp
inline CEditorDoc* CEditorView::GetDocument() const
   { return reinterpret_cast<CEditorDoc*>(m_pDocument); }
#endif

