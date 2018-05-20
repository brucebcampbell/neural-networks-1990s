// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "bpwin.h"

#include "MainFrm.h"
#include "TopologyDlg.h"
#include "LayerPages.h"
#include "TrainPages.h"
#include "WarningDlg.h"
#include "algorithm.h"
#include "Splash.h"
#include "InfoDlg.h"
#include "TreeViewPane.h"
#include "MainViewPane.h"

extern network ntwk;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_NETWORK_TOPOLOGY, OnNetworkTopology)
	ON_COMMAND(ID_NETWORK_LAYER, OnNetworkLayer)
	ON_COMMAND(ID_NETWORK_TRAINING, OnNetworkTraining)
	ON_COMMAND(ID_NETWORK_INFO, OnNetworkInfo)
	ON_COMMAND(ID_NETWORK_TRAIN, OnNetworkTrain)
	ON_COMMAND(ID_NETWORK_RUN, OnNetworkRun)
	ON_COMMAND(ID_NETWORK_RUNSETUP, OnNetworkRunsetup)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnNetworkTopology() 
{
	CTopologyDlg dlg;
	dlg.DoModal();
	//call apply function that loads data into network class
	
}

void CMainFrame::OnNetworkLayer() 
{
	// TODO: Add your command handler code here
	if((ntwk.layers!=3 && ntwk.layers!=2) || ntwk.input_dim==0)
	{
		//bbc add a nice warning dialog later
		CLayerWarningDlg layerWarning;
		layerWarning.DoModal();
		return;
	}
	switch(ntwk.layers)
	{
	case 3:
		{
			CPropertySheet layersSheet("Layers Setup");

			CLayer1Page pgLayer1;
			CLayer2Page pgLayer2;
			CLayer3Page pgLayer3;

			layersSheet.AddPage(&pgLayer1);
			layersSheet.AddPage(&pgLayer2);
			layersSheet.AddPage(&pgLayer3);

			layersSheet.DoModal();
			break;
		}
	case 2:
		{
			CPropertySheet layersSheet("Layers Setup");

			CLayer1Page pgLayer1;
			CLayer2Page pgLayer2;

			layersSheet.AddPage(&pgLayer1);
			layersSheet.AddPage(&pgLayer2);

			layersSheet.DoModal();
			break;
		}
	default:
		{
			AfxMessageBox("There has been a program error");
			exit(1);
		}
	}

	
	
}

void CMainFrame::OnNetworkTraining() 
{
	//check that the topology is setup properly 
	if(ntwk.layers==3)  //check for three layer setup
	{
		if( ((ntwk.layer_p)->units<1)   ||
			((ntwk.layer_p+1)->units<1) ||
			((ntwk.layer_p+2)->units<1) ) 
		{
			//bbc add a nice warning dialog later
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}
	else // check for two layer setup
	{
		if( ((ntwk.layer_p)->units<1) ||
			((ntwk.layer_p+1)->units<1) ) 
		{
			//bbc add a nice warning dialog later
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}


	CPropertySheet trainingSheet("Training Setup");

	CBkpPage pgBkp;
//	CLmsPage pgLms;
	CMdPage pgMd;

	
	trainingSheet.AddPage(&pgBkp);
//	trainingSheet.AddPage(&pgLms); 
	trainingSheet.AddPage(&pgMd);

	trainingSheet.DoModal();
}

void CMainFrame::OnNetworkInfo() 
{
	CInfoDlg dlg;
	dlg.DoModal();
	
}

void CMainFrame::OnNetworkTrain() 
{
	if((ntwk.layers!=3 && ntwk.layers!=2) || ntwk.input_dim==0)
	{
		//bbc add a nice warning dialog later
		CLayerWarningDlg layerWarning;
		layerWarning.DoModal();
		return;
	}
	if(ntwk.layers==3)  //check for three layer setup
	{
		if( ((ntwk.layer_p)->units<1)   ||
			((ntwk.layer_p+1)->units<1) ||
			((ntwk.layer_p+2)->units<1) ) 
		{
			//bbc add a nice warning dialog later
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}
	else // check for two layer setup
	{
		if( ((ntwk.layer_p)->units<1) ||
			((ntwk.layer_p+1)->units<1) ) 
		{
			//bbc add a nice warning dialog later
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}

	switch(ntwk.training_type)
	{
	case network::BKP:
		{
			//check that we don't have theshold activation since it's not differentiable
			if(ntwk.layers==2)
			{
				for(int i=0;i<2;i++)
				{
					if((ntwk.layer_p+i)->node_type==layer::THRESHOLD)
					{//go back and change activation
						AfxMessageBox("Cant Have THRESHOLD activation on any layer of a backprop network");
						CPropertySheet layersSheet("Layers Setup");
						CLayer1Page pgLayer1;
						CLayer2Page pgLayer2;
						layersSheet.AddPage(&pgLayer1);
						layersSheet.AddPage(&pgLayer2);
						layersSheet.DoModal();
					}
				}
			}
			if(ntwk.layers==3)
			{
				for(int i=0;i<3;i++)
				{//go back and change activation on offendin layer
					if((ntwk.layer_p+i)->node_type==layer::THRESHOLD)
					{
						AfxMessageBox("Cant Have THRESHOLD activation on any layer of a backprop network");
						CPropertySheet layersSheet("Layers Setup");
						CLayer1Page pgLayer1;
						CLayer2Page pgLayer2;
						CLayer3Page pgLayer3;
						layersSheet.AddPage(&pgLayer1);
						layersSheet.AddPage(&pgLayer2);
						layersSheet.AddPage(&pgLayer3);
						layersSheet.DoModal();
					}
				}
			}
			if(ntwk.passes<=0)
			{
				AfxMessageBox("Need to have at least one training pass.You have set it to zero or a negative number");
				CPropertySheet trainingSheet("Training Setup");
				CBkpPage pgBkp;
				trainingSheet.AddPage(&pgBkp);
				trainingSheet.DoModal();
			}
			if(ntwk.training_param<=0)
			{
				AfxMessageBox("The training parameter has to be a positive number");
				CPropertySheet trainingSheet("Training Setup");
				CBkpPage pgBkp;
				trainingSheet.AddPage(&pgBkp);
				trainingSheet.DoModal();
			}
			if(ntwk.input_dim<=0)
			{
				AfxMessageBox("The input dim has to be greater than 0");
				CTopologyDlg dlg;
				dlg.DoModal();
			}
			if((ntwk.upper_range-ntwk.lower_range)<=0)
			{
				AfxMessageBox("The upper range must be greater than the lower range");
				CPropertySheet trainingSheet("Training Setup");
				CBkpPage pgBkp;
				trainingSheet.AddPage(&pgBkp);
				trainingSheet.DoModal();
			}
			if( (ntwk.input_file_buf[0]=='\0') || (ntwk.output_file_buf[0]=='\0') )
			{
				AfxMessageBox("A valid input and or output file is missing");
				CPropertySheet trainingSheet("Training Setup");
				CBkpPage pgBkp;
				trainingSheet.AddPage(&pgBkp);
				trainingSheet.DoModal();
			}
			CInfoDlg dlg;
			dlg.DoModal();

			win_train();
			break;
		}
	
	case network::MD:
		{
			//check that we don't have theshold activation since it's not differentiable
			if(ntwk.layers==2)
			{
				for(int i=0;i<2;i++)
				{
					if((ntwk.layer_p+i)->node_type!=layer::THRESHOLD)
					{//go back and change activation
						AfxMessageBox("Can't have any activation except THRESHOLD on a minimum disturbance network");
						CPropertySheet layersSheet("Layers Setup");
						CLayer1Page pgLayer1;
						CLayer2Page pgLayer2;
						layersSheet.AddPage(&pgLayer1);
						layersSheet.AddPage(&pgLayer2);
						layersSheet.DoModal();
					}
				}
			}
			if(ntwk.layers==3)
			{
				for(int i=0;i<3;i++)
				{//go back and change activation on offendin layer
					if((ntwk.layer_p+i)->node_type!=layer::THRESHOLD)
					{
						AfxMessageBox("Can't have any activation except THRESHOLD on a minimum disturbance network");
						CPropertySheet layersSheet("Layers Setup");
						CLayer1Page pgLayer1;
						CLayer2Page pgLayer2;
						CLayer3Page pgLayer3;
						layersSheet.AddPage(&pgLayer1);
						layersSheet.AddPage(&pgLayer2);
						layersSheet.AddPage(&pgLayer3);
						layersSheet.DoModal();
					}
				}
			}
			if(ntwk.passes<=0)
			{
				AfxMessageBox("Need to have at least one training pass.You have set it to zero or a negative number");
				CPropertySheet trainingSheet("Training Setup");
				CMdPage pgMd;
				trainingSheet.AddPage(&pgMd);
				trainingSheet.DoModal();
			}
			if(ntwk.training_param<=0)
			{
				AfxMessageBox("The training parameter has to be a positive number");
				CPropertySheet trainingSheet("Training Setup");
				CMdPage pgMd;
				trainingSheet.AddPage(&pgMd);
				trainingSheet.DoModal();
			}
			if(ntwk.input_dim<=0)
			{
				AfxMessageBox("The input dim has to be greater than 1");
				CTopologyDlg dlg;
				dlg.DoModal();
			}
			if((ntwk.upper_range-ntwk.lower_range)<=0)
			{
				AfxMessageBox("The upper range must be greater than the lower range");
				CPropertySheet trainingSheet("Training Setup");
				CMdPage pgMd;
				trainingSheet.AddPage(&pgMd);
				trainingSheet.DoModal();
			}
			if( (ntwk.input_file_buf[0]=='\0') || (ntwk.output_file_buf[0]=='\0') )
			{
				AfxMessageBox("A valid input and or output file is missing");
				CPropertySheet trainingSheet("Training Setup");
				CMdPage pgMd;
				trainingSheet.AddPage(&pgMd);
				trainingSheet.DoModal();
			}
			CInfoDlg dlg;
			dlg.DoModal();
			win_train();
			break;
		}
	default:
		{
			AfxMessageBox("No functionality for any training type other than backprop and min_diturbance...exiting");
			exit(1);
		}
		}
}

void CMainFrame::OnNetworkRun() 
{

	if(ntwk.input_dim==0)
	{
		CLayerWarningDlg layerWarning;
		layerWarning.DoModal();
	}
	if(ntwk.layers==3)  //check for three layer setup
	{
		if( ((ntwk.layer_p)->units<1)   ||
			((ntwk.layer_p+1)->units<1) ||
			((ntwk.layer_p+2)->units<1) ) 
		{
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}
	else // check for two layer setup
	{
		if( ((ntwk.layer_p)->units<1) ||
			((ntwk.layer_p+1)->units<1) ) 
		{
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}

	if( (ntwk.training_type!=network::RUN) || (ntwk.input_file_buf[0]=='\0') || (ntwk.output_file_buf[0]=='\0') ||(ntwk.weights_file_buf[0]=='\0'))
	{
		AfxMessageBox("Please check that the input, output and weight files are correct before proceeding to run");
		CRunDlg dlg;
		dlg.DoModal();
		ntwk.training_type=network::RUN;
	}
	win_train();
	
}

void CMainFrame::OnNetworkRunsetup() 
{
	if(ntwk.layers==3)  //check for three layer setup
	{
		if( ((ntwk.layer_p)->units<1)   ||
			((ntwk.layer_p+1)->units<1) ||
			((ntwk.layer_p+2)->units<1) ) 
		{
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}
	else // check for two layer setup
	{
		if( ((ntwk.layer_p)->units<1) ||
			((ntwk.layer_p+1)->units<1) ) 
		{
			CTrainingWarning trainingWarning;
			trainingWarning.DoModal();
			return;
		}
	}
	CRunDlg dlg;
	dlg.DoModal();

}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CTreeViewPane), CSize(130, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CMainViewPane), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
	
	return TRUE;
}
