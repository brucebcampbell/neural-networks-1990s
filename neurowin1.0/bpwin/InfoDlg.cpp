// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bpwin.h"
#include "InfoDlg.h"
#include "algorithm.h"
#include "topologyDlg.h"
extern network ntwk;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog


CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoDlg)
	DDX_Control(pDX, IDC_LIST_LAYER3, m_layer3ListCtrl);
	DDX_Control(pDX, IDC_LIST_LAYER2, m_layer2ListCtrl);
	DDX_Control(pDX, IDC_LIST_LAYER1, m_layer1ListCtrl);
	DDX_Control(pDX, IDC_VARIABLE_LIST, m_varListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg message handlers

BOOL CInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//load image list
	m_greekSmall.Create(IDB_GREEKLETTERS,16,1,RGB(255,255,255) );
	m_greekSubscriptSmall.Create(IDB_GREEKLETTERSSUB,16,1,RGB(255,255,255) ); //for layers list ctrl
    //set image lists for the list controls
	m_varListCtrl.SetImageList(&m_greekSmall,LVSIL_SMALL);
	m_layer1ListCtrl.SetImageList(&m_greekSubscriptSmall,LVSIL_SMALL);
	m_layer2ListCtrl.SetImageList(&m_greekSubscriptSmall,LVSIL_SMALL);
	m_layer3ListCtrl.SetImageList(&m_greekSubscriptSmall,LVSIL_SMALL);
	//create LV colums
	LV_COLUMN varListColumn;
	LV_ITEM varListItem;
	char *arColumns[2]={"Variable","Value"};
	varListColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	varListColumn.fmt=LVCFMT_LEFT;
	varListColumn.cx=145;
	for(int nColumn=0;nColumn<2;nColumn++)
	{
		varListColumn.iSubItem=nColumn;
		varListColumn.pszText=arColumns[nColumn];
		m_varListCtrl.InsertColumn(nColumn,&varListColumn);
	}
	//add list items
	varListItem.mask=LVIF_TEXT|LVIF_IMAGE;
	varListItem.iSubItem=0;

	char *arVariables[7]={"input_dim","layers","passes","training_param","upper_weight_init","lower_weight_init","training_vectors"};
	CString arValues[7];
	arValues[0].Format("%d",ntwk.input_dim);
	arValues[1].Format("%d",ntwk.layers);
	arValues[2].Format("%d",ntwk.passes);
	arValues[3].Format("%lf",ntwk.training_param);
    arValues[4].Format("%lf",ntwk.upper_range);
	arValues[5].Format("%lf",ntwk.lower_range);
	arValues[6].Format("%d",ntwk.num_vectors);

	for(int nItem=0;nItem<7;nItem++)
	{
		varListItem.iItem=nItem;
		varListItem.iImage=nItem;
		varListItem.pszText=arVariables[nItem];
		m_varListCtrl.InsertItem(&varListItem);
		m_varListCtrl.SetItemText(nItem,1,arValues[nItem]);
	}
	//fill up list ctrl for layer 1
	LV_COLUMN layer1ListColumn;
	LV_ITEM layer1ListItem;
	layer1ListColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	layer1ListColumn.fmt=LVCFMT_LEFT;
	layer1ListColumn.cx=135;
	for(int nColumn=0;nColumn<2;nColumn++)
	{
		layer1ListColumn.iSubItem=nColumn;
		layer1ListColumn.pszText=arColumns[nColumn];
		m_layer1ListCtrl.InsertColumn(nColumn,&layer1ListColumn);
	}
	//add list items
	layer1ListItem.mask=LVIF_TEXT|LVIF_IMAGE;
	layer1ListItem.iSubItem=0;

	char *arLayerVariables[3]={"bias","untis","activation_function"};
	arValues[0].Format("%lf",(ntwk.layer_p+0)->bias);
	arValues[1].Format("%d",(ntwk.layer_p+0)->units);
	switch((ntwk.layer_p+0)->node_type)
	{
	case 0:
		{
			arValues[2].Format("%s","LINEAR");
			break;
		}
	case 1:
		{
			arValues[2].Format("%s","LOGISTIC");
			break;
		}
	case 2:
		{
			arValues[2].Format("%s","TANH");
			break;
		}
	case 3:
		{
			arValues[2].Format("%s","THRESHOLD");
			break;
		}
	default:
		{
			AfxMessageBox("Program error: exiting");
			exit(1);

		}
	}



	for(int nItem=0;nItem<3;nItem++)
	{
		layer1ListItem.iItem=nItem;
		layer1ListItem.iImage=nItem;
		layer1ListItem.pszText=arLayerVariables[nItem];
		m_layer1ListCtrl.InsertItem(&layer1ListItem);
		m_layer1ListCtrl.SetItemText(nItem,1,arValues[nItem]);
	}//end fill layer 1 list ctrl

	//fill up list ctrl for layer 2
	LV_COLUMN layer2ListColumn;
	LV_ITEM layer2ListItem;
	layer2ListColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	layer2ListColumn.fmt=LVCFMT_LEFT;
	layer2ListColumn.cx=135;
	for(int nColumn=0;nColumn<2;nColumn++)
	{
		layer2ListColumn.iSubItem=nColumn;
		layer2ListColumn.pszText=arColumns[nColumn];
		m_layer2ListCtrl.InsertColumn(nColumn,&layer2ListColumn);
	}
	//add list items
	layer2ListItem.mask=LVIF_TEXT|LVIF_IMAGE;
	layer2ListItem.iSubItem=0;

	arValues[0].Format("%lf",(ntwk.layer_p+1)->bias);
	arValues[1].Format("%d",(ntwk.layer_p+1)->units);
	switch((ntwk.layer_p+1)->node_type)
	{
	case 0:
		{
			arValues[2].Format("%s","LINEAR");
			break;
		}
	case 1:
		{
			arValues[2].Format("%s","LOGISTIC");
			break;
		}
	case 2:
		{
			arValues[2].Format("%s","TANH");
			break;
		}
	case 3:
		{
			arValues[2].Format("%s","THRESHOLD");
			break;
		}
	default:
		{
			AfxMessageBox("Program error: exiting");
			exit(1);

		}
	}


	for(int nItem=0;nItem<3;nItem++)
	{
		layer2ListItem.iItem=nItem;
		layer2ListItem.iImage=nItem;
		layer2ListItem.pszText=arLayerVariables[nItem];
		m_layer2ListCtrl.InsertItem(&layer2ListItem);
		m_layer2ListCtrl.SetItemText(nItem,1,arValues[nItem]);
	}//end fill layer 2 list ctrl
	if(ntwk.layers!=2)
	{
		//fill up list ctrl for layer 3
		LV_COLUMN layer3ListColumn;
		LV_ITEM layer3ListItem;
		layer3ListColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
		layer3ListColumn.fmt=LVCFMT_LEFT;
		layer3ListColumn.cx=135;
		for(int nColumn=0;nColumn<2;nColumn++)
		{
			layer3ListColumn.iSubItem=nColumn;
			layer3ListColumn.pszText=arColumns[nColumn];
			m_layer3ListCtrl.InsertColumn(nColumn,&layer3ListColumn);
		}
		//add list items
		layer3ListItem.mask=LVIF_TEXT|LVIF_IMAGE;
		layer3ListItem.iSubItem=0;
	
		arValues[0].Format("%lf",(ntwk.layer_p+2)->bias);
		arValues[1].Format("%d",(ntwk.layer_p+2)->units);
		switch((ntwk.layer_p+2)->node_type)
		{
		case 0:
			{
				arValues[2].Format("%s","LINEAR");
				break;
			}
		case 1:
			{
				arValues[2].Format("%s","LOGISTIC");
				break;
			}
		case 2:
			{
				arValues[2].Format("%s","TANH");
				break;
			}
		case 3:
			{
				arValues[2].Format("%s","THRESHOLD");
				break;
			}
		default:
			{
				AfxMessageBox("Program error: exiting");
				exit(1);

			}
		}


		for(int nItem=0;nItem<3;nItem++)
		{
			layer3ListItem.iItem=nItem;
			layer3ListItem.iImage=nItem;
			layer3ListItem.pszText=arLayerVariables[nItem];
			m_layer3ListCtrl.InsertItem(&layer3ListItem);
			m_layer3ListCtrl.SetItemText(nItem,1,arValues[nItem]);
		}//end fill layer 3 list ctrl
	}//end if !2 layers
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





