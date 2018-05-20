// bpwinView.cpp : implementation of the CBpwinView class
//

#include "stdafx.h"
#include "bpwin.h"

#include "bpwinDoc.h"
#include "bpwinView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBpwinView

IMPLEMENT_DYNCREATE(CBpwinView, CView)

BEGIN_MESSAGE_MAP(CBpwinView, CView)
	//{{AFX_MSG_MAP(CBpwinView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBpwinView construction/destruction

CBpwinView::CBpwinView()
{
	// TODO: add construction code here

}

CBpwinView::~CBpwinView()
{
}

BOOL CBpwinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBpwinView drawing

void CBpwinView::OnDraw(CDC* pDC)
{
	CBpwinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBpwinView printing

BOOL CBpwinView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBpwinView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBpwinView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBpwinView diagnostics

#ifdef _DEBUG
void CBpwinView::AssertValid() const
{
	CView::AssertValid();
}

void CBpwinView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBpwinDoc* CBpwinView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBpwinDoc)));
	return (CBpwinDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBpwinView message handlers
