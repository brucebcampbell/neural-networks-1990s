#if !defined(AFX_INFODLG_H__D6C21D21_C857_11D0_972E_444553540000__INCLUDED_)
#define AFX_INFODLG_H__D6C21D21_C857_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog

class CInfoDlg : public CDialog
{
// Construction
public:
	CInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoDlg)
	enum { IDD = IDD_INFODISP };
	CListCtrl	m_layer3ListCtrl;
	CListCtrl	m_layer2ListCtrl;
	CListCtrl	m_layer1ListCtrl;
	CListCtrl	m_varListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CImageList m_greekSmall;
	CImageList m_greekSubscriptSmall;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODLG_H__D6C21D21_C857_11D0_972E_444553540000__INCLUDED_)
