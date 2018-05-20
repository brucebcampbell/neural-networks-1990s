// bpwinView.h : interface of the CBpwinView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BPWINVIEW_H__AE6E820D_C785_11D0_972E_444553540000__INCLUDED_)
#define AFX_BPWINVIEW_H__AE6E820D_C785_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBpwinView : public CView
{
protected: // create from serialization only
	CBpwinView();
	DECLARE_DYNCREATE(CBpwinView)

// Attributes
public:
	CBpwinDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBpwinView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBpwinView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBpwinView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in bpwinView.cpp
inline CBpwinDoc* CBpwinView::GetDocument()
   { return (CBpwinDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPWINVIEW_H__AE6E820D_C785_11D0_972E_444553540000__INCLUDED_)
