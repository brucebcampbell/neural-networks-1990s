#if !defined(AFX_PROGRESSDLG_H__5CC88421_DC00_11D0_972E_444553540000__INCLUDED_)
#define AFX_PROGRESSDLG_H__5CC88421_DC00_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBkpProgressDlg dialog

class CBkpProgressDlg : public CDialog
{
// Construction
public:
	CBkpProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBkpProgressDlg)
	enum { IDD = IDD_BKP_PROGRESS };
	CProgressCtrl	m_bkpProgressCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBkpProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBkpProgressDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnStartTraining();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMdProgressDlg dialog

class CMdProgressDlg : public CDialog
{
// Construction
public:
	CMdProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMdProgressDlg)
	enum { IDD = IDD_MD_PROGRESS };
	CProgressCtrl	m_mdProgressCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMdProgressDlg)
	afx_msg void OnStartTraining();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CRunProgressDlg dialog

class CRunProgressDlg : public CDialog
{
// Construction
public:
	CRunProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRunProgressDlg)
	enum { IDD = IDD_RN_PROGRESS };
	CProgressCtrl	m_rnProgressCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunProgressDlg)
	afx_msg void OnStartTraining();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__5CC88421_DC00_11D0_972E_444553540000__INCLUDED_)
