#if !defined(AFX_TOPOLOGYDLG_H__AE6E8215_C785_11D0_972E_444553540000__INCLUDED_)
#define AFX_TOPOLOGYDLG_H__AE6E8215_C785_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TopologyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTopologyDlg dialog

class CTopologyDlg : public CDialog
{
// Construction
public:
	CTopologyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTopologyDlg)
	enum { IDD = IDD_SET_TOPOLOGY };
	CEdit	m_trainPasses;
	CEdit	m_trainParam;
	CEdit	m_inputDim;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopologyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTopologyDlg)
	virtual void OnOK();
	afx_msg void On2layers();
	afx_msg void On3layers();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOPOLOGYDLG_H__AE6E8215_C785_11D0_972E_444553540000__INCLUDED_)
