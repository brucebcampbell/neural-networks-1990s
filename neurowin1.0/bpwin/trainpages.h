#if !defined(AFX_TRAINPAGES_H__9C7B6FE1_D1B7_11D0_972E_444553540000__INCLUDED_)
#define AFX_TRAINPAGES_H__9C7B6FE1_D1B7_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrainPages.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBkpPage dialog

class CBkpPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBkpPage)

// Construction
public:
	CBkpPage();
	~CBkpPage();

// Dialog Data
	//{{AFX_DATA(CBkpPage)
	enum { IDD = IDD_BKP_PAGE };
	CEdit	m_trainNumVectors;
	CEdit	m_dataFile;
	CEdit	m_outputFile;
	CEdit	m_upperWeight;
	CEdit	m_lowerWeight;
	CEdit	m_trainPasses;
	CEdit	m_trainParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBkpPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBkpPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTrainParam();
	afx_msg void OnChangeTrainPasses();
	afx_msg void OnChangeUpperWeight();
	afx_msg void OnChangeLowerWeight();
	afx_msg void OnInputFileBtn();
	afx_msg void OnOutputFileBtn();
	afx_msg void OnChangeNumVectors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CLmsPage dialog

class CLmsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLmsPage)

// Construction
public:
	CLmsPage();
	~CLmsPage();

// Dialog Data
	//{{AFX_DATA(CLmsPage)
	enum { IDD = IDD_LMS_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLmsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLmsPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CMdPage dialog

class CMdPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMdPage)

// Construction
public:
	CMdPage();
	~CMdPage();

// Dialog Data
	//{{AFX_DATA(CMdPage)
	enum { IDD = IDD_MD_PAGE };
	CEdit	m_upperWeight;
	CEdit	m_trainPasses;
	CEdit	m_trainParam;
	CEdit	m_outputFile;
	CEdit	m_trainNumVectors;
	CEdit	m_lowerWeight;
	CEdit	m_dataFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMdPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMdPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnInputFileBtn();
	afx_msg void OnOutputFileBtn();
	afx_msg void OnChangeLowerWeight();
	afx_msg void OnChangeTrainParam();
	afx_msg void OnChangeTrainPasses();
	afx_msg void OnChangeUpperWeight();
	afx_msg void OnChangeNumVectors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CRunDlg dialog

class CRunDlg : public CDialog
{
// Construction
public:
	CRunDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRunDlg)
	enum { IDD = IDD_RUN_SETUP };
	CEdit	m_weightsFile;
	CEdit	m_outputFile;
	CEdit	m_runNumVectors;
	CEdit	m_dataFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnInputFileBtn();
	afx_msg void OnOutputFileBtn();
	afx_msg void OnWeightsFileBtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINPAGES_H__9C7B6FE1_D1B7_11D0_972E_444553540000__INCLUDED_)
