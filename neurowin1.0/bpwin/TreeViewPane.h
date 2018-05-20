#if !defined(AFX_TREEVIEWPANE_H__F2FCB9A2_FEBC_11D0_972E_444553540000__INCLUDED_)
#define AFX_TREEVIEWPANE_H__F2FCB9A2_FEBC_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TreeViewPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeViewPane view

class CTreeViewPane : public CTreeView
{
protected:
	CTreeViewPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTreeViewPane)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeViewPane)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTreeViewPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeViewPane)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEVIEWPANE_H__F2FCB9A2_FEBC_11D0_972E_444553540000__INCLUDED_)
