#if !defined(AFX_LAYERPAGES_H__1E792FC1_CB5D_11D0_972E_444553540000__INCLUDED_)
#define AFX_LAYERPAGES_H__1E792FC1_CB5D_11D0_972E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LayerPages.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLayer3Page dialog

class CLayer3Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CLayer3Page)

// Construction
public:
	CLayer3Page();
	~CLayer3Page();

// Dialog Data
	//{{AFX_DATA(CLayer3Page)
	enum { IDD = IDD_SET_LAYER3 };
	CSpinButtonCtrl	m_layer3Node;
	CEdit	m_layer3Bias;
	int		m_layer3Units;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLayer3Page)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLayer3Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnLayer3Linear();
	afx_msg void OnLayer3Logistic();
	afx_msg void OnLayer3Tanh();
	afx_msg void OnChangeLayer3Bias();
	afx_msg void OnChangeLayer3Nodes();
	afx_msg void OnLayer3Threshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CLayer2Page dialog

class CLayer2Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CLayer2Page)

// Construction
public:
	CLayer2Page();
	~CLayer2Page();

// Dialog Data
	//{{AFX_DATA(CLayer2Page)
	enum { IDD = IDD_SET_LAYER2 };
	CSpinButtonCtrl	m_layer2Node;
	CEdit	m_layer2Bias;
	int		m_layer2Units;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLayer2Page)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLayer2Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnLayer2Linear();
	afx_msg void OnLayer2Logistic();
	afx_msg void OnLayer2Tanh();
	afx_msg void OnChangeLayer2Bias();
	afx_msg void OnChangeLayer2Nodes();
	afx_msg void OnLayer2Threshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CLayer1Page dialog

class CLayer1Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CLayer1Page)

// Construction
public:
	CLayer1Page();
	~CLayer1Page();

// Dialog Data
	//{{AFX_DATA(CLayer1Page)
	enum { IDD = IDD_SET_LAYER1 };
	CEdit	m_layer1Bias;
	CSpinButtonCtrl	m_layer1Node;
	int		m_layer1Units;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLayer1Page)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLayer1Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnLayer1Linear();
	afx_msg void OnLayer1Logistic();
	afx_msg void OnLayer1Tanh();
	afx_msg void OnChangeLayer1Bias();
	afx_msg void OnChangeLayer1Nodes();
	afx_msg void OnLayer1Threshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERPAGES_H__1E792FC1_CB5D_11D0_972E_444553540000__INCLUDED_)
