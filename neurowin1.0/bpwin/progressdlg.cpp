// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "ProgressDlg.h"
#include "algorithm.h"
#include "backprop.h"
#include "mindist.h"
#include "runntwk.h"
extern network ntwk;
extern bkp *bkp_ntwk;
extern md *md_ntwk;
extern rnntwk *rn_ntwk;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBkpProgressDlg dialog


CBkpProgressDlg::CBkpProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBkpProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBkpProgressDlg)
	//}}AFX_DATA_INIT
}


void CBkpProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBkpProgressDlg)
	DDX_Control(pDX, IDC_TRAINING_PROGRESS, m_bkpProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBkpProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CBkpProgressDlg)
	ON_BN_CLICKED(IDC_START_TRAINING, OnStartTraining)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBkpProgressDlg message handlers

void CBkpProgressDlg::OnCancel() 
{
	CDialog::OnCancel();
}

BOOL CBkpProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bkpProgressCtrl.SetRange(0,bkp_ntwk->passes);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBkpProgressDlg::OnStartTraining() 
{
	bkp_ntwk->back_prop(); //need to add some handler or thread to deal with taking
	     //user input while training is going on.
	
	//if training is not interupted, we call OnOK() to end dialog
	OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CMdProgressDlg dialog


CMdProgressDlg::CMdProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMdProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMdProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMdProgressDlg)
	DDX_Control(pDX, IDC_TRAINING_PROGRESS, m_mdProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMdProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CMdProgressDlg)
	ON_BN_CLICKED(IDC_START_TRAINING, OnStartTraining)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdProgressDlg message handlers

void CMdProgressDlg::OnStartTraining() 
{
	md_ntwk->min_disturbance(); //need to add some handler or thread to deal with taking
	//user input while training is going on.
	
	//if training is not interupted, we call OnOK() to end dialog
	OnOK();
}

void CMdProgressDlg::OnCancel() 
{
	CDialog::OnCancel();
}

BOOL CMdProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_mdProgressCtrl.SetRange(0,md_ntwk->passes);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CRunProgressDlg dialog


CRunProgressDlg::CRunProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRunProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunProgressDlg)
	DDX_Control(pDX, IDC_RUNNING_PROGRESS, m_rnProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CRunProgressDlg)
	ON_BN_CLICKED(IDC_START_TRAINING, OnStartTraining)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunProgressDlg message handlers

void CRunProgressDlg::OnStartTraining() 
{
	rn_ntwk->run_network(); //need to add some handler or thread to deal with taking
	//user input while training is going on.
	
	//if training is not interupted, we call OnOK() to end dialog
	OnOK();
	
}

BOOL CRunProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_rnProgressCtrl.SetRange(0,rn_ntwk->num_vectors);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunProgressDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
