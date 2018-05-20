// WarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "WarningDlg.h"
extern int test;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerWarningDlg dialog


CLayerWarningDlg::CLayerWarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerWarningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerWarningDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLayerWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerWarningDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerWarningDlg, CDialog)
	//{{AFX_MSG_MAP(CLayerWarningDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerWarningDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CTrainingWarning dialog


CTrainingWarning::CTrainingWarning(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainingWarning::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainingWarning)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTrainingWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainingWarning)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainingWarning, CDialog)
	//{{AFX_MSG_MAP(CTrainingWarning)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainingWarning message handlers
/////////////////////////////////////////////////////////////////////////////
// CMdZeroInputwarningDlg dialog


CMdZeroInputwarningDlg::CMdZeroInputwarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdZeroInputwarningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMdZeroInputwarningDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMdZeroInputwarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMdZeroInputwarningDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMdZeroInputwarningDlg, CDialog)
	//{{AFX_MSG_MAP(CMdZeroInputwarningDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdZeroInputwarningDlg message handlers

void CMdZeroInputwarningDlg::OnCancel() 
{//test is bool test from min dist module
	test=0;
	
	CDialog::OnCancel();
}

void CMdZeroInputwarningDlg::OnOK() 
{
	test=1;
	
	
	CDialog::OnOK();
}
