// TrainPages.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "TrainPages.h"
#include "algorithm.h"
#include "backprop.h"
#include "infodlg.h"
extern network ntwk;  //object created in algorithm.cpp
extern bkp bkp_network;  //object created in mainframe.cpp

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBkpPage property page

IMPLEMENT_DYNCREATE(CBkpPage, CPropertyPage)

CBkpPage::CBkpPage() : CPropertyPage(CBkpPage::IDD)
{
	//{{AFX_DATA_INIT(CBkpPage)
	//}}AFX_DATA_INIT
}

CBkpPage::~CBkpPage()
{
}

void CBkpPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBkpPage)
	DDX_Control(pDX, IDC_NUM_VECTORS, m_trainNumVectors);
	DDX_Control(pDX, IDC_DATA_FILE_EDIT, m_dataFile);
	DDX_Control(pDX, IDC_OUTPUT_FILE_EDIT, m_outputFile);
	DDX_Control(pDX, IDC_UPPER_WEIGHT, m_upperWeight);
	DDX_Control(pDX, IDC_LOWER_WEIGHT, m_lowerWeight);
	DDX_Control(pDX, IDC_TRAIN_PASSES, m_trainPasses);
	DDX_Control(pDX, IDC_TRAIN_PARAM, m_trainParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBkpPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBkpPage)
	ON_EN_CHANGE(IDC_TRAIN_PARAM, OnChangeTrainParam)
	ON_EN_CHANGE(IDC_TRAIN_PASSES, OnChangeTrainPasses)
	ON_EN_CHANGE(IDC_UPPER_WEIGHT, OnChangeUpperWeight)
	ON_EN_CHANGE(IDC_LOWER_WEIGHT, OnChangeLowerWeight)
	ON_BN_CLICKED(IDC_INPUT_FILE_BTN, OnInputFileBtn)
	ON_BN_CLICKED(IDC_OUTPUT_FILE_BTN, OnOutputFileBtn)
	ON_EN_CHANGE(IDC_NUM_VECTORS, OnChangeNumVectors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLmsPage property page

IMPLEMENT_DYNCREATE(CLmsPage, CPropertyPage)

CLmsPage::CLmsPage() : CPropertyPage(CLmsPage::IDD)
{
	//{{AFX_DATA_INIT(CLmsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLmsPage::~CLmsPage()
{
}

void CLmsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLmsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLmsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLmsPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLmsPage message handlers

/////////////////////////////////////////////////////////////////////////////
// CBkpPage message handlers
BOOL CBkpPage::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString szEdit;	
	m_trainParam.GetWindowText(szEdit);
	ntwk.training_param=atof(szEdit);

	m_trainPasses.GetWindowText(szEdit);
	ntwk.passes=atoi(szEdit);

	m_upperWeight.GetWindowText(szEdit);
	ntwk.upper_range=atof(szEdit);

	m_lowerWeight.GetWindowText(szEdit);
	ntwk.lower_range=atof(szEdit);

	m_trainNumVectors.GetWindowText(szEdit);
	ntwk.num_vectors=atoi(szEdit);

	m_dataFile.GetWindowText(szEdit);
	strcpy(ntwk.input_file_buf,szEdit);

	m_outputFile.GetWindowText(szEdit);
	strcpy(ntwk.output_file_buf,szEdit);

	return CPropertyPage::OnApply();
}

BOOL CBkpPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString input;
	input.Format("%d",ntwk.passes);
	m_trainPasses.SetWindowText(input);

	input.Format("%lf",ntwk.training_param);
	m_trainParam.SetWindowText(input);

	input.Format("%lf",ntwk.upper_range);
	m_upperWeight.SetWindowText(input);

	input.Format("%lf",ntwk.lower_range);
	m_lowerWeight.SetWindowText(input);

	input.Format("%d",ntwk.num_vectors);
	m_trainNumVectors.SetWindowText(input);

	input.Format("%s",ntwk.input_file_buf);
	m_dataFile.SetWindowText(input);

	input.Format("%s",ntwk.output_file_buf);
	m_outputFile.SetWindowText(input);

	ntwk.training_type=network::BKP;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBkpPage::OnChangeTrainParam() 
{
	SetModified();
	
}

void CBkpPage::OnChangeTrainPasses() 
{
	SetModified();
}

void CBkpPage::OnChangeUpperWeight() 
{
	SetModified();
	
}

void CBkpPage::OnChangeLowerWeight() 
{
	SetModified();
	
}

void CBkpPage::OnChangeNumVectors() 
{
	SetModified();
	
}

void CBkpPage::OnInputFileBtn() 
{
	CFileDialog fileDlg(TRUE,"csv");
	fileDlg.DoModal();
	m_dataFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
}

void CBkpPage::OnOutputFileBtn() 
{
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();
	m_outputFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
}



/////////////////////////////////////////////////////////////////////////////
// CMdPage property page

IMPLEMENT_DYNCREATE(CMdPage, CPropertyPage)

CMdPage::CMdPage() : CPropertyPage(CMdPage::IDD)
{
	//{{AFX_DATA_INIT(CMdPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMdPage::~CMdPage()
{
}

void CMdPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMdPage)
	DDX_Control(pDX, IDC_UPPER_WEIGHT, m_upperWeight);
	DDX_Control(pDX, IDC_TRAIN_PASSES, m_trainPasses);
	DDX_Control(pDX, IDC_TRAIN_PARAM, m_trainParam);
	DDX_Control(pDX, IDC_OUTPUT_FILE_EDIT, m_outputFile);
	DDX_Control(pDX, IDC_NUM_VECTORS, m_trainNumVectors);
	DDX_Control(pDX, IDC_LOWER_WEIGHT, m_lowerWeight);
	DDX_Control(pDX, IDC_DATA_FILE_EDIT, m_dataFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMdPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMdPage)
	ON_BN_CLICKED(IDC_INPUT_FILE_BTN, OnInputFileBtn)
	ON_BN_CLICKED(IDC_OUTPUT_FILE_BTN, OnOutputFileBtn)
	ON_EN_CHANGE(IDC_LOWER_WEIGHT, OnChangeLowerWeight)
	ON_EN_CHANGE(IDC_TRAIN_PARAM, OnChangeTrainParam)
	ON_EN_CHANGE(IDC_TRAIN_PASSES, OnChangeTrainPasses)
	ON_EN_CHANGE(IDC_UPPER_WEIGHT, OnChangeUpperWeight)
	ON_EN_CHANGE(IDC_NUM_VECTORS, OnChangeNumVectors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdPage message handlers

BOOL CMdPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString input;
	input.Format("%d",ntwk.passes);
	m_trainPasses.SetWindowText(input);

	input.Format("%lf",ntwk.training_param);
	m_trainParam.SetWindowText(input);

	input.Format("%lf",ntwk.upper_range);
	m_upperWeight.SetWindowText(input);

	input.Format("%lf",ntwk.lower_range);
	m_lowerWeight.SetWindowText(input);

	input.Format("%d",ntwk.num_vectors);
	m_trainNumVectors.SetWindowText(input);

	input.Format("%s",ntwk.input_file_buf);
	m_dataFile.SetWindowText(input);

	input.Format("%s",ntwk.output_file_buf);
	m_outputFile.SetWindowText(input);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMdPage::OnInputFileBtn() 
{
	CFileDialog fileDlg(TRUE,"csv");
	fileDlg.DoModal();
	m_dataFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
}

void CMdPage::OnOutputFileBtn() 
{
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();
	m_outputFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
}

BOOL CMdPage::OnApply() 
{
	CString szEdit;	
	m_trainParam.GetWindowText(szEdit);
	ntwk.training_param=atof(szEdit);

	m_trainPasses.GetWindowText(szEdit);
	ntwk.passes=atoi(szEdit);

	m_upperWeight.GetWindowText(szEdit);
	ntwk.upper_range=atof(szEdit);

	m_lowerWeight.GetWindowText(szEdit);
	ntwk.lower_range=atof(szEdit);

	m_trainNumVectors.GetWindowText(szEdit);
	ntwk.num_vectors=atoi(szEdit);

	m_dataFile.GetWindowText(szEdit);
	strcpy(ntwk.input_file_buf,szEdit);

	m_outputFile.GetWindowText(szEdit);
	strcpy(ntwk.output_file_buf,szEdit);

	ntwk.training_type=network::MD;
	
	return CPropertyPage::OnApply();
}

void CMdPage::OnChangeLowerWeight() 
{
	SetModified();
}

void CMdPage::OnChangeTrainParam() 
{
	SetModified();
}

void CMdPage::OnChangeTrainPasses() 
{
	SetModified();
}

void CMdPage::OnChangeUpperWeight() 
{
	SetModified();
}

void CMdPage::OnChangeNumVectors() 
{
	SetModified();
}
/////////////////////////////////////////////////////////////////////////////
// CRunDlg dialog


CRunDlg::CRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunDlg)
	DDX_Control(pDX, IDC_WEIGHTS_FILE_EDIT, m_weightsFile);
	DDX_Control(pDX, IDC_OUTPUT_FILE_EDIT, m_outputFile);
	DDX_Control(pDX, IDC_NUM_VECTORS, m_runNumVectors);
	DDX_Control(pDX, IDC_DATA_FILE_EDIT, m_dataFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunDlg, CDialog)
	//{{AFX_MSG_MAP(CRunDlg)
	ON_BN_CLICKED(IDC_INPUT_FILE_BTN, OnInputFileBtn)
	ON_BN_CLICKED(IDC_OUTPUT_FILE_BTN, OnOutputFileBtn)
	ON_BN_CLICKED(IDC_WEIGHTS_FILE_BTN, OnWeightsFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunDlg message handlers

BOOL CRunDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString input;
	input.Format("%s",ntwk.input_file_buf);
	m_dataFile.SetWindowText(input);

	input.Format("%s",ntwk.output_file_buf);
	m_outputFile.SetWindowText(input);

	input.Format("%s",ntwk.weights_file_buf);
	m_weightsFile.SetWindowText(input);

	input.Format("%d",ntwk.num_vectors);
	m_runNumVectors.SetWindowText(input);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunDlg::OnInputFileBtn() 
{
	CFileDialog fileDlg(TRUE,"csv");
	fileDlg.DoModal();
	m_dataFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
	
}

void CRunDlg::OnOutputFileBtn() 
{
	CFileDialog fileDlg(TRUE,"csv");
	fileDlg.DoModal();
	m_outputFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
	
}

void CRunDlg::OnWeightsFileBtn() 
{
	CFileDialog fileDlg(TRUE,"csv");
	fileDlg.DoModal();
	m_weightsFile.SetWindowText(fileDlg.m_ofn.lpstrFile);
	//now add code to create file pointer or handle to write to
	
}

void CRunDlg::OnOK() 
{
	CString szEdit;
	m_runNumVectors.GetWindowText(szEdit);
	ntwk.num_vectors=atoi(szEdit);

	m_dataFile.GetWindowText(szEdit);
	strcpy(ntwk.input_file_buf,szEdit);

	m_outputFile.GetWindowText(szEdit);
	strcpy(ntwk.output_file_buf,szEdit);

	m_weightsFile.GetWindowText(szEdit);
	strcpy(ntwk.weights_file_buf,szEdit);

	ntwk.training_type=network::RUN;

	
	CDialog::OnOK();
}
