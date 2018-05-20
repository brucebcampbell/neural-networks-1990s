// bpwinDoc.cpp : implementation of the CBpwinDoc class
//

#include "stdafx.h"
#include "bpwin.h"

#include "bpwinDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBpwinDoc

IMPLEMENT_DYNCREATE(CBpwinDoc, CDocument)

BEGIN_MESSAGE_MAP(CBpwinDoc, CDocument)
	//{{AFX_MSG_MAP(CBpwinDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBpwinDoc construction/destruction

CBpwinDoc::CBpwinDoc()
{
	// TODO: add one-time construction code here

}

CBpwinDoc::~CBpwinDoc()
{
}

BOOL CBpwinDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBpwinDoc serialization

void CBpwinDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBpwinDoc diagnostics

#ifdef _DEBUG
void CBpwinDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBpwinDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBpwinDoc commands
