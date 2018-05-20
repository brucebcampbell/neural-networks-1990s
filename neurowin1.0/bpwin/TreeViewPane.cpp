// TreeViewPane.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "TreeViewPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeViewPane

IMPLEMENT_DYNCREATE(CTreeViewPane, CTreeView)

CTreeViewPane::CTreeViewPane()
{
}

CTreeViewPane::~CTreeViewPane()
{
}


BEGIN_MESSAGE_MAP(CTreeViewPane, CTreeView)
	//{{AFX_MSG_MAP(CTreeViewPane)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeViewPane drawing

void CTreeViewPane::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewPane diagnostics

#ifdef _DEBUG
void CTreeViewPane::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTreeViewPane::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeViewPane message handlers
