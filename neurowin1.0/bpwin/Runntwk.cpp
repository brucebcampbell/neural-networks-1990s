//BEGIN WIN specific
#include "stdafx.h"
#include "bpwin.h"
#include "algorithm.h"
#include "runntwk.h"
#include "Progressdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CRunProgressDlg rnProgress;
//END WIN specific
extern network ntwk;
//extern rnntwk *rnntwk_ntwk;  //I beleve this is here only so we can see it in the debugger?
//figure out which of these we really need in the c++ ver
#include <math.h>
//#include<ctype.h> 
#include<io.h> 
extern int *num_weights;
long int rn_save_position;
rn_unit_train::rn_unit_train()
{
	//new must call the default constructor when initializing arrays of objects rn_unit_train
	weights=new double[*num_weights];
}

rn_unit_train::~rn_unit_train()
{
	delete[] weights;
}

rnntwk::rnntwk(network *ntwk_input)
{
	eof_flag=0;
	num_weights=new int;//this is required so that constructor
	//for rn_unit_train knows woh many weights to allocate, we can't pass it because 
	//arrays of obj's call default constructor
	input_dim=ntwk_input->input_dim;
	layers=ntwk_input->layers;
	num_vectors=ntwk_input->num_vectors;
	strcpy(input_file_buf,ntwk_input->input_file_buf);
	strcpy(output_file_buf,ntwk_input->output_file_buf);
	strcpy(weights_file_buf,ntwk_input->weights_file_buf);

	if(layers==2)
		output_dim=(ntwk_input->layer_p+1)->units;
	else
		output_dim=(ntwk_input->layer_p+2)->units;
	
	//fill up the layer_p class using layer assignment operator
	for(int i=0;i<layers;i++)
		*(layer_p+i)=*(ntwk_input->layer_p+i);

	data_p=new double[input_dim];

    //make unit_train items
    rn_unit_p=new rn_unit_train*[layers];

    for(count_l=0;count_l<layers;count_l++)  
	// alocate space for weights remembering that bias weight is always the first one 
    {
        if(count_l==0)
        {
			*num_weights=input_dim+1;//so def const for rn_unit_train obj sets up correct size array
            rn_unit_p[count_l]=new rn_unit_train[(layer_p+count_l)->units];
        }
        if(count_l!=0)  // allocate array of weight vectors for each rn_unit_train in the remaining layers 
        {
			*num_weights=(layer_p+count_l-1)->units + 1;
			rn_unit_p[count_l]=new rn_unit_train[(layer_p+count_l)->units];
        }
    }
    

}

rnntwk::~rnntwk()
{

	for(int i=layers-1;i>=0;i--)//delete layers of rn_unit_train
	{
		delete[] rn_unit_p[i];
	}
	delete[] rn_unit_p;
	delete[] data_p;
	delete num_weights;
}

void rnntwk::run_network()
{
	rn_file_open();
	// initialize the weights for matrix of struct rn_unit_train 
    for(count_l=0;count_l<layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                for(count_w=0;count_w<input_dim+1;count_w++)
                {
                    *((*(rn_unit_p+count_l)+count_u)->weights+count_w)= \
						rn_get_weight();
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                for(count_w=0;count_w<(layer_p+count_l-1)->units+1;count_w++)
                {
                    *((*(rn_unit_p+count_l)+count_u)->weights+count_w)= \
						rn_get_weight();
                }
            }
        }
    }
	//end setup array of unit-train objects	

	count=num_vectors;
    while(eof_flag!=1)
    {
		count--;
		//BEGIN WINSPECIFIC		
		rnProgress.m_rnProgressCtrl.SetPos(count);
		//END WINSPECIFIC
        eof_flag=rn_get_data();
        rn_propagate();
        rn_write_output();
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
}

void rnntwk::rn_propagate()
{
    double temp=0,sum=0;
    for(count_l=0;count_l<layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                sum=layer_p->bias *  *((*(rn_unit_p+count_l)+count_u)->weights);
                for(count_w=1;count_w<=input_dim;count_w++)
                {
                    sum+= *(data_p+count_w-1) *  *((*(rn_unit_p+count_l)+count_u)->weights+count_w);  
                }
                // place the output 
                switch(layer_p->node_type)  // add the rest of the activation funcitons  
                {
                    case THRESHOLD:
                    {
                        if(sum>=0)
                        temp=1;
                        if(sum<0)
                        temp=-1;
                        (*(rn_unit_p+count_l)+count_u)->activation=temp;
                         break;
                    }
                    case LINEAR:
                    {
                        (*(rn_unit_p+count_l)+count_u)->activation=sum;
                        break;
                    }
                    case LOGISTIC:
                    {
                        (*(rn_unit_p+count_l)+count_u)->activation=1/(1+exp(-sum));
                        break;
                    }
                    case TANH:
                    {
                        (*(rn_unit_p+count_l)+count_u)->activation=tanh(sum);
                        break;
                    }
                    default:
                    {
                        AfxMessageBox("ERROR: did not recgonize node_type presented to run_ntwk routine.");
                        exit(1);
                    }
                }// end switch 

            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                sum=(layer_p+count_l)->bias *  *((*(rn_unit_p+count_l)+count_u)->weights);
                for(count_w=1;count_w<=(layer_p+count_l-1)->units;count_w++)
                {
                    sum+= (*(rn_unit_p+count_l-1)+count_w-1)->activation * \
						*((*(rn_unit_p+count_l)+count_u)->weights+count_w);
                    
                }
                // place the output 
                switch(layer_p->node_type)  //this can be moved to the end of the  
                {
                    case THRESHOLD:
                    {
                        if(sum>=0)
                        temp=1;
                        if(sum<0)
                        temp=-1;
                        (*(rn_unit_p+count_l)+count_u)->activation=temp;
                        break;
                    }
                    case LINEAR:
                    {
                        (*(rn_unit_p+count_l)+count_u)->activation=sum;
                        break;
                    }
                    case LOGISTIC:
                    {
                        (*(rn_unit_p+count_l)+count_u)->activation=1/(1+exp(-sum));
                        break;
                    }
                    case TANH:
                    {
                        (*(rn_unit_p+count_l)+count_u)->activation=tanh(sum); 
                        break;
                    }
                    default:
                    {
                        AfxMessageBox("ERROR: did not recgonize node_type presented to run_ntwk routine.");
                        exit(1);
                    }
                }// end switch 

            }
        }
    }//end count layers
}

void rnntwk::rn_write_output()  // writes the data vector, and the output vector as one record 
{
    fprintf(fp1,"input=:");
    for(count_w=0;count_w<input_dim;count_w++)
    {
        fprintf(fp1,"%lg,",*(data_p+count_w));
    }

    fprintf(fp1,"\toutput=:");
    for(count_u=0;count_u<output_dim;count_u++)
    {
        if(count_u!=output_dim-1)
        {
            fprintf(fp1,"%lg,", (*(rn_unit_p+layers-1)+count_u)->activation);
        }
        if(count_u==output_dim-1)
        {
            fprintf(fp1,"%lg\n",(*(rn_unit_p+layers-1)+count_u)->activation);
        }
    }
}


double rnntwk::rn_get_weight()
{
    double weight;
    while(!feof(fp2))  // get training info 
    {
        buf[0]=fgetc(fp2);
        if(buf[0]==':')
        break;
        if(feof(fp2))
        {
            AfxMessageBox("ERROR: unexpected end of file.");
            exit(1);
        }
    }
    fscanf(fp2,"%lg",&weight);    

    return weight;
    
}

int rnntwk::rn_get_data()
{
    int flag=0;
    
    for(count_w=0;count_w<input_dim;count_w++)  //  get data vactor 
    {
        fscanf(fp,"%lg",(data_p+count_w));
        while(!feof(fp) && count_w!=input_dim-1) 
        {
            buf[0]=fgetc(fp);
            if(buf[0]==',')
            break;
            if(feof(fp))
            {
                AfxMessageBox("ERROR: unexpected end of file.");
                exit(1);
            }
        }            
     }
     
    // we now neet to verify that we're at the end of a record,  or at least scan whitespace until data is encountered 
    buf[0]='\n';
    while(isspace(buf[0]))
    {
        rn_save_position=ftell(fp);
        buf[0]=fgetc(fp);
        if(feof(fp))  // end run when EOF is encountered at the end of a record,  ERROR above for EOF before complete record is entered 
        {
            flag=1;
        }
        if(buf[0]==',') // check that we are indeed at the end of a record 
        {
            AfxMessageBox("ERROR: unexpected field delimiter - expecting end of record");
            exit(1);
        }
   }
   fseek(fp,rn_save_position,SEEK_SET); // put fp back one since data was encounterd on last read and we're set to scan next vector 
   return flag;
}

void rnntwk::rn_file_open(void)  
{
	//windows specific
    while((fp=fopen(input_file_buf,"r+"))==NULL)
    {
		AfxMessageBox("Error opening data file.");
		exit(1);
    }
	while((fp1=fopen(output_file_buf,"a+"))==NULL)
    {
        AfxMessageBox("Error opening output file.");
		exit(1);
    }
	while((fp2=fopen(weights_file_buf,"r+"))==NULL)
    {
		AfxMessageBox("Error opening weights file.");
		exit(1);   
    }

}

 



