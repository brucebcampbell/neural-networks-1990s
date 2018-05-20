//BEGIN WIN specific
#include "stdafx.h"
#include "bpwin.h"
#include "ProgressDlg.h"
#include "algorithm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//above must be removed for release application to call regular new
//for now all call to new are replaced by a debug version of new
extern CBkpProgressDlg bkpProgress; //created in algorithm.h to call back_prop() fn
//END WIN specific
#include "backprop.h"
#include "rand.h"
extern network ntwk;
extern bkp *bkp_ntwk;  //I beleve this is here only so we can see it in the debugger?

//figure out which of these we really need in the c++ ver
//#include<stdio.h>
//#include<stdlib.h>
#include <math.h>
//#include<ctype.h> 
#include<io.h> 

int *num_weights;


bp_unit_train::bp_unit_train()
{
	//new must call the default constructor when initializing arrays of objects bp_unit_train
	weights=new double[*num_weights];
}

bp_unit_train::~bp_unit_train()
{
	delete[] weights;
}


bkp::bkp(network *ntwk_input)
{
	num_weights=new int;
	rand_init();
	
	//set up the member var's with construction data - make a copy constructor for this later
	input_dim=ntwk_input->input_dim;
	passes=ntwk_input->passes;
	layers=ntwk_input->layers;
	lower_range=ntwk_input->lower_range;
	num_vectors=ntwk_input->num_vectors;
	training_param=ntwk_input->training_param;
	upper_range=ntwk_input->upper_range;
	strcpy(input_file_buf,ntwk_input->input_file_buf);
	strcpy(output_file_buf,ntwk_input->output_file_buf);

	if(layers==2)
		output_dim=(ntwk_input->layer_p+1)->units;
	else
		output_dim=(ntwk_input->layer_p+2)->units;
	
	//fill up the layer_p class using layer assignment operator
	for(int i=0;i<layers;i++)
		*(layer_p+i)=*(ntwk_input->layer_p+i);

	global_error=new double*[passes];
	for(int i=0;i<passes;i++)
		global_error[i]=new double[num_vectors];
	
	data_p=new double[input_dim];
	
    desired_out=new double[output_dim];

    //make unit_train items
    bp_unit_p=new bp_unit_train*[layers];

    for(count_l=0;count_l<layers;count_l++)  
	// alocate space for weights remembering that bias weight is always the first one 
    {
        if(count_l==0)
        {
			*num_weights=input_dim+1;//so def const for bp_unit_train obj sets up correct size array
            bp_unit_p[count_l]=new bp_unit_train[(layer_p+count_l)->units];
        }
        if(count_l!=0)  // allocate array of weight vectors for each bp_unit_train in the remaining layers 
        {
			*num_weights=(layer_p+count_l-1)->units + 1;
			bp_unit_p[count_l]=new bp_unit_train[(layer_p+count_l)->units];
        }
    }
    // initialize the weights for matrix of struct bp_unit_train 
    for(count_l=0;count_l<layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                for(count_w=0;count_w<input_dim+1;count_w++)
                {
                    *((*(bp_unit_p+count_l)+count_u)->weights+count_w)= \
						lower_range+rand2()*(upper_range-lower_range);
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                for(count_w=0;count_w<(layer_p+count_l-1)->units+1;count_w++)
                {
                    *((*(bp_unit_p+count_l)+count_u)->weights+count_w)= \
						lower_range+rand2()*(upper_range-lower_range);
                }
            }
        }
    }
//end setup array of unit-train objects	

}

bkp::~bkp()
{

	for(int i=layers-1;i>=0;i--)//delete layers of bp_unit_train
	{
		delete[] bp_unit_p[i];
	}
	delete[] bp_unit_p;
	delete[] desired_out;
	delete[] data_p;
	for(int i=passes-1;i>=0;i--)
		delete[] *(global_error+i);
	delete[] global_error;
	delete num_weights;
	rand_cleanup();//deletes seed created in rand_init
}

void bkp::operator=(bkp &source)
{
	//for now we only chain to the assignment operator for the base class
	network::operator=(source);  //source is cast to a pointer of type network
}


  
void bkp::back_prop()
{
	bp_file_open();

    // training routine,  runs through training vectors number of times specified in network info file 
    for(int pass_num=0;pass_num<passes;pass_num++)
    {
		//fprintf(fp3,"pass=%d",pass_num);
		//BEGIN WINSPECIFIC
		if(pass_num%10==0)
			bkpProgress.m_bkpProgressCtrl.SetPos(pass_num);
		//END WINSPECIFIC
        for(vector_number=0;vector_number<num_vectors;vector_number++)
        {
            // puts data from fp into data_p and desired_out  
            bp_get_data(); 
            
            // bp_propagate the input,  the output is present at both bp_unit_p and layer_p 
            bp_propagate(data_p);  
  
            error=bp_error_comp(desired_out);  //  compute the bipolar error 

            *(*(global_error+pass_num)+vector_number)=error;

            bp_update_weight();
        }      
        rewind(fp);
    } 
 
	//insert some output or serialization fns here
    //info is in unit_train objects and error vectors

    fclose(fp);
	bp_output_network();
	fclose(fp1);
}


void bkp::bp_propagate(double *data_p)
{
    double sum;
    for(count_l=0;count_l<layers;count_l++)
    {
        if(count_l==0)
        {
            // go trought the frist layer, applying input vector to each node and computing outputs 
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                x=*(bp_unit_p+count_l)+count_u;
                
                sum=(layer_p)->bias * *(x->weights);  // compute bias term 

                /* We have to remember that we're using count_w to step through the units 
                   on the pervious layer which start at 0, but we're multiplying by non-bias weights 
                   which start at 1, so we subtract off 1 from the count in the first term of the sum */ 

                for(count_w=1;count_w<=input_dim;count_w++)
                    sum+=*(data_p+count_w-1) * *(x->weights+count_w);
              
                x->linear_output=sum;
                 
                switch(layer_p->node_type)   
                {
					case THRESHOLD:
                    {
                        AfxMessageBox("Program error: trying to train backprop network with threshold activation function. exiting...");
						exit(1);
                    }
                    case LINEAR:
                    {
                        x->activation=x->linear_output;
                        x->derivative=1;
                        break;
                    }
                    case LOGISTIC:
                    {
                        x->activation=1/(1+exp(-x->linear_output));
                        x->derivative=x->activation * (1-x->activation);
                        break;
                    }
                    case TANH:
                    {
                        x->activation=tanh(x->linear_output);
                        x->derivative=1-pow(x->activation,2); 
                        break;
                    }
                    default:
                    {
                        AfxMessageBox("ERROR: did not recgonize node_type presented to min_disturbance training.");
                        exit(1);
                    }
                }// end switch 
            }// end unit count for layer 0 
        }// end if for layer 0 
            
        if(count_l!=0)
        {
            
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                x=*(bp_unit_p+count_l)+count_u;
                
                sum=(layer_p+count_l)->bias * *(x->weights);  // bias term
                 
                for(count_w=1;count_w<=(layer_p+count_l-1)->units;count_w++)
                    sum+= (*(bp_unit_p+count_l-1)+count_w-1)->activation * *(x->weights+count_w);
                    
                x->linear_output=sum;
                    
                switch((layer_p+count_l)->node_type)
                {
                    case THRESHOLD:
                    {
						AfxMessageBox("Program error: trying to train backprop network with threshold activation function. exiting...");
                        exit(1);
                    }
                    case LINEAR:
                    {
                        x->activation=x->linear_output;
                        x->derivative=1;
                        break;
                    }
                    case LOGISTIC:
                    {
                        x->activation=1/(1+exp(-x->linear_output));
                        x->derivative=x->activation * (1-x->activation);
                        break;
                    }
                    case TANH:
                    {
                        x->activation=tanh(x->linear_output);
                        x->derivative=1-pow(x->activation,2); 
                        break;
                    }
                    
                    default:
                    {
						AfxMessageBox("ERROR: did not recognize activation type in propagate()");
                        exit(1);
                    }
               }// end switch 
            }// end unit count for layer count_l 
        }// end if for layer !=0 
    }// end layers count 
}    

// computes errors - stores in the unit_p structure 
double bkp::bp_error_comp(double *desired_out)  
{
    int count_out,count_hidden1;
    double temp,err;

    for(count_l=layers-1;count_l>=0;count_l--)
    {
        if(layers==3)
        {
            switch(count_l)
            {
                case 2:
                {
                    temp=0;
                    for(count_u=0;count_u<(layer_p+2)->units;count_u++)
                    {
                        temp+=pow((*(bp_unit_p+2)+count_u)->activation - *(desired_out+count_u),2);
                        (*(bp_unit_p+2)+count_u)->delta= \
							(*(desired_out+count_u) - (*(bp_unit_p+2)+count_u)->activation) * (*(bp_unit_p+2)+count_u)->derivative;
                    }
                    err=temp/2;
                    break;
                }
                case 1:
                {
                    for(count_u=0;count_u<(layer_p+1)->units;count_u++)
                    {
                        temp=0;
                        for(count_out=0;count_out<(layer_p+2)->units;count_out++)
                        {
                            temp+=(*(bp_unit_p+2)+count_out)->delta * *((*(bp_unit_p+2)+count_out)->weights+count_u+1);
                        }
                        (*(bp_unit_p+1)+count_u)->delta=(*(bp_unit_p+1)+count_u)->derivative * temp;
                    }
                    break;
                }
                case 0:
                {
                    for(count_u=0;count_u<(layer_p+0)->units;count_u++)
                    {
                        temp=0;
                        for(count_hidden1=0;count_hidden1<(layer_p+1)->units;count_hidden1++)
                        {
                                temp+=(*(bp_unit_p+1)+count_hidden1)->delta * *((*(bp_unit_p+1)+count_hidden1)->weights+count_u+1);
                        }
                        (*(bp_unit_p)+count_u)->delta=(*(bp_unit_p)+count_u)->derivative * temp;
                    }  
                    break;
                }
                default:
                {
                    AfxMessageBox("Program error, exiting...sorry");
                    exit(1);
                }
            }// end switch */
        }// end if layer==3 */

        if(layers==2)
        {
            switch(count_l)
            {
                case 1:
                {
                    temp=0;
                    for(count_u=0;count_u<(layer_p+1)->units;count_u++)
                    {
                        temp+=pow((*(bp_unit_p+1)+count_u)->activation - *(desired_out+count_u),2);
                        (*(bp_unit_p+1)+count_u)->delta= \
							(*(desired_out+count_u) - (*(bp_unit_p+1)+count_u)->activation) * (*(bp_unit_p+1)+count_u)->derivative;
                    }
                    err=temp/2;
                    break;
                }
                case 0:
                {
                    for(count_u=0;count_u<(layer_p)->units;count_u++)
                    {
                        temp=0;
                        for(count_out=0;count_out<(layer_p+1)->units;count_out++)
                        {
                            temp+=(*(bp_unit_p+1)+count_out)->delta * *((*(bp_unit_p+1)+count_out)->weights+count_u+1);
                        }
                        (*(bp_unit_p)+count_u)->delta=(*(bp_unit_p)+count_u)->derivative * temp;
                    }
                    break;
                }
                default:
				{
					AfxMessageBox("Program error, exiting...sorry");
                    exit(1);
                }
            }// end switch 
        }// end if layer==2           
    }//end count_l 
    return err;
}

void bkp::bp_update_weight()
{
    for(count_l=0;count_l<layers;count_l++)
    {
        if(count_l==0)
        {
            // go trough the first layer, applying input vector to each node and computing outputs 
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                x=*(bp_unit_p)+count_u;
                
                *(x->weights)+=training_param * x->delta * layer_p->bias;
                
                for(count_w=1;count_w<=input_dim;count_w++)
                {
					//fprintf(fp3,"layer=%d,unit=%d,weight=%d=:%lg\n",count_l,count_u,count_w,*((*(bp_unit_p+count_l)+count_u)->weights+count_w));
                    *(x->weights+count_w)+=training_param * x->delta * *(data_p+count_w-1);
                }
            }// end unit count for layer 0 
        }// end if for layer 0 
            
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                x=*(bp_unit_p+count_l)+count_u;

                *(x->weights)+=training_param * x->delta * (layer_p+count_l)->bias;
                
                for(count_w=1;count_w<=(layer_p+count_l-1)->units;count_w++)
                {
					//fprintf(fp3,"layer=%d,unit=%d,weight=%d=:%lg\n",count_l,count_u,count_w,*((*(bp_unit_p+count_l)+count_u)->weights+count_w));
                    *(x->weights+count_w)+=training_param * x->delta * (*(bp_unit_p+count_l-1)+count_w-1)->activation;
                }    
            }// end unit count for layer count_l 
        }// end if for layer !=0 
    }// end layers count     
}


void bkp::bp_get_data(void)
{
    for(count_w=0;count_w<input_dim;count_w++)  // get training vector 
    {
        fscanf(fp,"%lg",(data_p+count_w));
        while(!feof(fp)) 
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
    for(count_w=0;count_w<output_dim;count_w++)  // get output vector vector 
    {
        fscanf(fp,"%lg",(desired_out+count_w));
        while(!feof(fp) && count_w!=output_dim-1) 
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
    // We now neet to verify that we're at the end of a record,  or at least scan whitespace until data is encountered 
    buf[0]='\n';
    while(isspace(buf[0]))
    {
        save_position=ftell(fp);
        buf[0]=fgetc(fp);
        if( (vector_number!=num_vectors-1) && (feof(fp)) ) // check that we have not reached EOF prematurely 
        {
            AfxMessageBox("ERROR: unexpected EOF in data file. Possible cause is that # training vectors doesn't equal # vectors in entered");
            exit(1);
        }
        if(buf[0]==',') // check that we are indeed at the end of a record 
        {
            AfxMessageBox("\nERROR: unexpected field delimiter in data file - expecting end of record");
            exit(1);
        }
   }
   fseek(fp,save_position,SEEK_SET); // put fp back one since data was encounterd on last read and we're set to scan next vector 
}


void bkp::bp_file_open(void)  
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
/*	while((fp3=fopen("TESTFILE.txt","a+"))==NULL)
    {
        AfxMessageBox("Error opening output file.");
		exit(1);
    }
*/
}
    
// This prints trained network to file specified by FILE *fp1 
void bkp::bp_output_network()
{
    int count_l_p,count_u_p,count_w_p;
    CTime t=CTime::GetCurrentTime();
	CString s=t.Format("%A, %B, %d, %Y");
	fprintf(fp1,"Backpropagation Training Run on [%s]",s);
    for(count_l_p=0;count_l_p<layers;count_l_p++)  // write the unit_train structure 
    {
        fprintf(fp1,"\n\t\t\tLAYER [%d]",count_l_p);
        if(count_l_p==0)
        {
            for(count_u_p=0;count_u_p<layer_p->units;count_u_p++)
            {
                fprintf(fp1,"\nweights   UINT[%d][%d]\n",count_l_p,count_u_p);
                for(count_w_p=0;count_w_p<=input_dim;count_w_p++)
                {
                    fprintf(fp1,"=:%lg\n",*((*(bp_unit_p+count_l_p)+count_u_p)->weights+count_w_p));
                }
            }
        }
        if(count_l_p!=0)
        {
            for(count_u_p=0;count_u_p<(layer_p+count_l_p)->units;count_u_p++)
            {
                fprintf(fp1,"\nweights   UINT[%d][%d]\n",count_l_p,count_u_p);
                for(count_w_p=0;count_w_p<=(layer_p+count_l_p-1)->units;count_w_p++)
                {
                    fprintf(fp1,"=:%lg\n",*((*(bp_unit_p+count_l_p)+count_u_p)->weights+count_w_p));
                }
                
            }
        }
    }
	fprintf(fp1,"\n\n");
}
