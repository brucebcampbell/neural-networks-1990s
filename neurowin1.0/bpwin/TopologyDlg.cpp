// TopologyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "TopologyDlg.h"
#include "algorithm.h"
#include "InfoDlg.h"
extern network ntwk;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopologyDlg dialog


CTopologyDlg::CTopologyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTopologyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTopologyDlg)
	//}}AFX_DATA_INIT
}


void CTopologyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTopologyDlg)
	DDX_Control(pDX, IDC_INPUTDIM, m_inputDim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTopologyDlg, CDialog)
	//{{AFX_MSG_MAP(CTopologyDlg)
	ON_BN_CLICKED(IDC_2LAYERS, On2layers)
	ON_BN_CLICKED(IDC_3LAYERS, On3layers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopologyDlg message handlers



void CTopologyDlg::OnOK() 
{
	//set the variables
	CString szEdit;

	m_inputDim.GetWindowText(szEdit);
	ntwk.input_dim=atoi(szEdit);
	//create info dialog to display variables set
	
	CDialog::OnOK();
}


void CTopologyDlg::On2layers() 
{
	ntwk.layers=2;	
}

void CTopologyDlg::On3layers() 
{
	ntwk.layers=3;
}

BOOL CTopologyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	//set default layers to be 2
    if(ntwk.layers==3)
	{
		CButton *rb3ptr=(CButton*)GetDlgItem(IDC_3LAYERS);
		rb3ptr->SetCheck(1);
	}
	else
	{
		CButton *rb2ptr=(CButton*)GetDlgItem(IDC_2LAYERS);
		rb2ptr->SetCheck(1);
	}
    
	CString input;
	input.Format("%d",ntwk.input_dim);
	m_inputDim.SetWindowText(input);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
