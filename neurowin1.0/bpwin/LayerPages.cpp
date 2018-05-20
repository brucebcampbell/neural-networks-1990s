// LayerPages.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "LayerPages.h"
#include "infodlg.h"
#include "algorithm.h"
extern network ntwk;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayer3Page property page

IMPLEMENT_DYNCREATE(CLayer3Page, CPropertyPage)

CLayer3Page::CLayer3Page() : CPropertyPage(CLayer3Page::IDD)
{
	//{{AFX_DATA_INIT(CLayer3Page)
	m_layer3Units = (ntwk.layer_p+2)->units;
	//}}AFX_DATA_INIT
}

CLayer3Page::~CLayer3Page()
{
}

void CLayer3Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayer3Page)
	DDX_Control(pDX, IDC_SPIN_LAYER3_NODE, m_layer3Node);
	DDX_Control(pDX, IDC_LAYER3_BIAS, m_layer3Bias);
	DDX_Text(pDX, IDC_LAYER3_NODES, m_layer3Units);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayer3Page, CPropertyPage)
	//{{AFX_MSG_MAP(CLayer3Page)
	ON_BN_CLICKED(IDC_LAYER3_LINEAR, OnLayer3Linear)
	ON_BN_CLICKED(IDC_LAYER3_LOGISTIC, OnLayer3Logistic)
	ON_BN_CLICKED(IDC_LAYER3_TANH, OnLayer3Tanh)
	ON_EN_CHANGE(IDC_LAYER3_BIAS, OnChangeLayer3Bias)
	ON_EN_CHANGE(IDC_LAYER3_NODES, OnChangeLayer3Nodes)
	ON_BN_CLICKED(IDC_LAYER3_THRESHOLD, OnLayer3Threshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayer3Page message handlers

/////////////////////////////////////////////////////////////////////////////
// CLayer2Page property page

IMPLEMENT_DYNCREATE(CLayer2Page, CPropertyPage)

CLayer2Page::CLayer2Page() : CPropertyPage(CLayer2Page::IDD)
{
	//{{AFX_DATA_INIT(CLayer2Page)
	m_layer2Units = (ntwk.layer_p+1)->units;
	//}}AFX_DATA_INIT
}

CLayer2Page::~CLayer2Page()
{
}

void CLayer2Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayer2Page)
	DDX_Control(pDX, IDC_SPIN_LAYER2_NODE, m_layer2Node);
	DDX_Control(pDX, IDC_LAYER2_BIAS, m_layer2Bias);
	DDX_Text(pDX, IDC_LAYER2_NODES, m_layer2Units);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayer2Page, CPropertyPage)
	//{{AFX_MSG_MAP(CLayer2Page)
	ON_BN_CLICKED(IDC_LAYER2_LINEAR, OnLayer2Linear)
	ON_BN_CLICKED(IDC_LAYER2_LOGISTIC, OnLayer2Logistic)
	ON_BN_CLICKED(IDC_LAYER2_TANH, OnLayer2Tanh)
	ON_EN_CHANGE(IDC_LAYER2_BIAS, OnChangeLayer2Bias)
	ON_EN_CHANGE(IDC_LAYER2_NODES, OnChangeLayer2Nodes)
	ON_BN_CLICKED(IDC_LAYER2_THRESHOLD, OnLayer2Threshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayer2Page message handlers


/////////////////////////////////////////////////////////////////////////////
// CLayer1Page property page

IMPLEMENT_DYNCREATE(CLayer1Page, CPropertyPage)

CLayer1Page::CLayer1Page() : CPropertyPage(CLayer1Page::IDD)
{
	//{{AFX_DATA_INIT(CLayer1Page)
	m_layer1Units = (ntwk.layer_p)->units;
	//}}AFX_DATA_INIT
}

CLayer1Page::~CLayer1Page()
{
}

void CLayer1Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayer1Page)
	DDX_Control(pDX, IDC_LAYER1_BIAS, m_layer1Bias);
	DDX_Control(pDX, IDC_SPIN_LAYER1_NODE, m_layer1Node);
	DDX_Text(pDX, IDC_LAYER1_NODES, m_layer1Units);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayer1Page, CPropertyPage)
	//{{AFX_MSG_MAP(CLayer1Page)
	ON_BN_CLICKED(IDC_LAYER1_LINEAR, OnLayer1Linear)
	ON_BN_CLICKED(IDC_LAYER1_LOGISTIC, OnLayer1Logistic)
	ON_BN_CLICKED(IDC_LAYER1_TANH, OnLayer1Tanh)
	ON_EN_CHANGE(IDC_LAYER1_BIAS, OnChangeLayer1Bias)
	ON_EN_CHANGE(IDC_LAYER1_NODES, OnChangeLayer1Nodes)
	ON_BN_CLICKED(IDC_LAYER1_THRESHOLD, OnLayer1Threshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayer1Page message handlers

BOOL CLayer1Page::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_layer1Node.SetRange(0,30000);
	//set default node type to be LINEAR
    if(ntwk.layer_p->node_type==layer::LOGISTIC)
	{
		CButton *rbptrLogistic=(CButton*)GetDlgItem(IDC_LAYER1_LOGISTIC);
		rbptrLogistic->SetCheck(1);
	}
	else if(ntwk.layer_p->node_type==layer::TANH)
	{
		CButton *rbptrTanh=(CButton*)GetDlgItem(IDC_LAYER1_TANH);
		rbptrTanh->SetCheck(1);
	}
	else if((ntwk.layer_p)->node_type==layer::THRESHOLD)
	{
		CButton *rbptrThreshold=(CButton*)GetDlgItem(IDC_LAYER1_THRESHOLD);
		rbptrThreshold->SetCheck(1);
	}
	else
	{
		CButton *rbptrLinear=(CButton*)GetDlgItem(IDC_LAYER1_LINEAR);
		rbptrLinear->SetCheck(1);
	}
    
	CString input;
	input.Format("%d",ntwk.layer_p->units);
	m_layer1Node.SetWindowText(input);

	input.Format("%lf",ntwk.layer_p->bias);
	m_layer1Bias.SetWindowText(input);



	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//side note : in C++ enumerators are only accessible to member fn's unless
//qualified with the class name.
void CLayer1Page::OnLayer1Linear() 
{
	ntwk.layer_p->node_type=layer::LINEAR;
	SetModified();
}

void CLayer1Page::OnLayer1Logistic() 
{
	ntwk.layer_p->node_type=layer::LOGISTIC;
	SetModified();
}

void CLayer1Page::OnLayer1Tanh() 
{
	ntwk.layer_p->node_type=layer::TANH;
	SetModified();
}

void CLayer1Page::OnLayer1Threshold() 
{
	ntwk.layer_p->node_type=layer::THRESHOLD;
	SetModified();
}


void CLayer1Page::OnChangeLayer1Bias() 
{
	SetModified();
	
}

void CLayer1Page::OnChangeLayer1Nodes() 
{
	SetModified();
}

BOOL CLayer2Page::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_layer2Node.SetRange(0,30000);
	//set default node type to be LINEAR
    if((ntwk.layer_p+1)->node_type==layer::LOGISTIC)
	{
		CButton *rbptrLogistic=(CButton*)GetDlgItem(IDC_LAYER2_LOGISTIC);
		rbptrLogistic->SetCheck(1);
	}
	else if((ntwk.layer_p+1)->node_type==layer::TANH)
	{
		CButton *rbptrTanh=(CButton*)GetDlgItem(IDC_LAYER2_TANH);
		rbptrTanh->SetCheck(1);
	}
	else if((ntwk.layer_p+1)->node_type==layer::THRESHOLD)
	{
		CButton *rbptrThreshold=(CButton*)GetDlgItem(IDC_LAYER2_THRESHOLD);
		rbptrThreshold->SetCheck(1);
	}
	else
	{
		CButton *rbptrLinear=(CButton*)GetDlgItem(IDC_LAYER2_LINEAR);
		rbptrLinear->SetCheck(1);
	}
    
	CString input;
	input.Format("%d",(ntwk.layer_p+1)->units);
	m_layer2Node.SetWindowText(input);

	input.Format("%lf",(ntwk.layer_p+1)->bias);
	m_layer2Bias.SetWindowText(input);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLayer2Page::OnLayer2Linear() 
{
	(ntwk.layer_p+1)->node_type=layer::LINEAR;
	SetModified();
}

void CLayer2Page::OnLayer2Logistic() 
{
	(ntwk.layer_p+1)->node_type=layer::LOGISTIC;
	SetModified();
}

void CLayer2Page::OnLayer2Tanh() 
{
	(ntwk.layer_p+1)->node_type=layer::TANH;
	SetModified();
}

void CLayer2Page::OnLayer2Threshold() 
{
	(ntwk.layer_p+1)->node_type=layer::THRESHOLD;
	SetModified();
}


void CLayer2Page::OnChangeLayer2Bias() 
{
	SetModified();
	
}

void CLayer2Page::OnChangeLayer2Nodes() 
{
	SetModified();
}

BOOL CLayer3Page::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_layer3Node.SetRange(0,30000);
	//set default node type to be LINEAR
    if((ntwk.layer_p+2)->node_type==layer::LOGISTIC)
	{
		CButton *rbptrLogistic=(CButton*)GetDlgItem(IDC_LAYER3_LOGISTIC);
		rbptrLogistic->SetCheck(1);
	}
	else if((ntwk.layer_p+2)->node_type==layer::TANH)
	{
		CButton *rbptrTanh=(CButton*)GetDlgItem(IDC_LAYER3_TANH);
		rbptrTanh->SetCheck(1);
	}
	else if((ntwk.layer_p+2)->node_type==layer::THRESHOLD)
	{
		CButton *rbptrThreshold=(CButton*)GetDlgItem(IDC_LAYER3_THRESHOLD);
		rbptrThreshold->SetCheck(1);
	}
	else
	{
		CButton *rbptrLinear=(CButton*)GetDlgItem(IDC_LAYER3_LINEAR);
		rbptrLinear->SetCheck(1);
	}
    
	CString input;
	input.Format("%d",(ntwk.layer_p+2)->units);
	m_layer3Node.SetWindowText(input);

	input.Format("%lf",(ntwk.layer_p+2)->bias);
	m_layer3Bias.SetWindowText(input);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLayer3Page::OnLayer3Linear() 
{
	(ntwk.layer_p+2)->node_type=layer::LINEAR;
	SetModified();
}

void CLayer3Page::OnLayer3Logistic() 
{
	(ntwk.layer_p+2)->node_type=layer::LOGISTIC;
	SetModified();
}

void CLayer3Page::OnLayer3Tanh() 
{
	(ntwk.layer_p+2)->node_type=layer::TANH;
	SetModified();
}

void CLayer3Page::OnLayer3Threshold() 
{
	(ntwk.layer_p+2)->node_type=layer::THRESHOLD;
	SetModified();
	
}

void CLayer3Page::OnChangeLayer3Bias() 
{
	SetModified();
}

void CLayer3Page::OnChangeLayer3Nodes() 
{
	SetModified();
}


BOOL CLayer1Page::OnApply() 
{
	CString szTemp;
	ntwk.layer_p->units=m_layer1Units;
	m_layer1Bias.GetWindowText(szTemp);
	ntwk.layer_p->bias=atof(szTemp);
	return CPropertyPage::OnApply();
}

BOOL CLayer2Page::OnApply() 
{
	CString szTemp;
	(ntwk.layer_p+1)->units=m_layer2Units;
	m_layer2Bias.GetWindowText(szTemp);
	(ntwk.layer_p+1)->bias=atof(szTemp);
	return CPropertyPage::OnApply();
}

BOOL CLayer3Page::OnApply() 
{
	CString szTemp;
	(ntwk.layer_p+2)->units=m_layer3Units;
	m_layer3Bias.GetWindowText(szTemp);
	(ntwk.layer_p+2)->bias=atof(szTemp);
	return CPropertyPage::OnApply();
}





