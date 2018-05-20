#include "stdafx.h"
#include "bpwin.h"
#include "algorithm.h"
#include "backprop.h"
#include "mindist.h"
#include "runntwk.h"
#include "Progressdlg.h"
#include "MainViewPane.h"
extern int *num_weights;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
layer::layer()
{
	bias=0;
	node_type=LINEAR;
	units=0;
}

layer::~layer()
{
	int a=0;
}

void layer::operator=(layer &source)
{
	units=source.units;
	node_type=source.node_type;
	bias=source.bias;
}

network::network()
{
	input_dim=0;
	layers=3;
	passes=0;
	training_param=0.01;
	upper_range=1;
	lower_range=0;
	layer_p=new layer[layers];
	input_file_buf[0]='\0';
	output_file_buf[0]='\0';
	weights_file_buf[0]='\0';

}

network::~network()
{
	delete[] layer_p;
}

void network::operator=(network &source)
{
	if(this==&source)
		return;
	layers=source.layers;
	input_dim=source.input_dim;
	training_param=source.training_param;
	num_vectors=source.num_vectors;
	passes=source.passes;
	lower_range=source.lower_range;
	upper_range=source.upper_range;

	//not so sure about this code
	for(int count=0;count<3;count++)
	{
		if(source.layers==2 && count==2)
			break;
		
	}

	//we need to cast to the layer since layer_p is a dynamic array
	//of layers and automatic pointer conversio dosen't resolve which one we mean
}


network ntwk;  //this class is filed by windows dialogs
bkp *bkp_ntwk;
md *md_ntwk;
rnntwk *rn_ntwk;

CBkpProgressDlg bkpProgress;  //these are global because back_prop() & min_disturbance()
CMdProgressDlg mdProgress;    //need access to m_***ProgessCtrl member fn to update progess contol in 
CRunProgressDlg rnProgress;

void win_train()
{
	//when this function is called, widows has inserted all of the
	//info necessary for a training run into the calss ntwk.  We now 
	//initialize the class bkp_ntwk and call the training member functions 
	//necessary.  Here we will also have serialization code if needed.

	network *ntwk_ptr;
	ntwk_ptr=&ntwk;

	// Declare the variables needed
#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
    oldMemState.Checkpoint();
#endif

switch (ntwk.training_type)
{
		
	case network::BKP:
	{
		bkp_ntwk=new bkp(ntwk_ptr);//construct bkp object
		bkpProgress.DoModal();
		//insert dialog to serialize data, and display error results in chart.
		delete bkp_ntwk;
		break;
	}
	case network::MD:
	{
		md_ntwk=new md(ntwk_ptr);//construct md object
		mdProgress.DoModal();
		delete md_ntwk;
		break;
	}
	case network::RUN:
	{
		rn_ntwk=new rnntwk(ntwk_ptr);//construct rnntwk object
		rnProgress.DoModal();
		delete rn_ntwk;
		break;
	}
	default:
	{
		AfxMessageBox("Program Error:Training type not recognized in iwn_train()");
		exit(1);
	}
}


#ifdef _DEBUG
    newMemState.Checkpoint();
	oldMemState.DumpAllObjectsSince();
    if( diffMemState.Difference( oldMemState, newMemState ) )
    {
        TRACE( "Memory leaked from md_ntwk object!\n" );
		AfxMessageBox("MEM LEAK FROM md_ntwk");
    }
#endif
	
}

