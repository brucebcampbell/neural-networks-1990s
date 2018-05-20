// MainViewPane.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "MainViewPane.h"
#include "bpwinDoc.h"
#include "algorithm.h"
extern network ntwk;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainViewPane

IMPLEMENT_DYNCREATE(CMainViewPane, CEditView)

CMainViewPane::CMainViewPane()
{
}

CMainViewPane::~CMainViewPane()
{
}


BEGIN_MESSAGE_MAP(CMainViewPane, CEditView)
	//{{AFX_MSG_MAP(CMainViewPane)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainViewPane drawing

void CMainViewPane::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int nLineHeight=tm.tmHeight+tm.tmExternalLeading;
	CPoint ptText(0,0);
	CString szOutput;
	FILE *fp;
	while((fp=fopen(ntwk.output_file_buf,"a+"))==NULL)
    {
        AfxMessageBox("Error opening file.");
		exit(1);
    }

	char line[256];
		
	while( fgets( line, 256, fp ) != NULL)
	{
		szOutput.Format("%s",line);
		pDC->TextOut(ptText.x,ptText.y,szOutput);
		ptText.y+=nLineHeight;
	}
	fclose(fp);
	pDoc->UpdateAllViews(this);
	


}

/////////////////////////////////////////////////////////////////////////////
// CMainViewPane diagnostics

#ifdef _DEBUG
void CMainViewPane::AssertValid() const
{
	CView::AssertValid();
}

void CMainViewPane::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainViewPane message handlers

BOOL CMainViewPane::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEditView::OnCommand(wParam, lParam);
}
