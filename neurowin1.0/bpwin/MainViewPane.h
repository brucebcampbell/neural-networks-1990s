#if !defined(AFX_MAINVIEWPANE_H__F2FCB9A1_FEBC_11D0_972E_444553540000__INCLUDED_)
#define AFX_MAINVIEWPANE_H__F2FCB9A1_FEBC_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MainViewPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainViewPane view

class CMainViewPane : public CEditView
{
protected:
	CMainViewPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMainViewPane)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainViewPane)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainViewPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainViewPane)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEWPANE_H__F2FCB9A1_FEBC_11D0_972E_444553540000__INCLUDED_)
