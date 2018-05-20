//BEGIN WIN specific
#include "stdafx.h"
#include "bpwin.h"
#include "ProgressDlg.h"
#include "warningdlg.h"
#include "algorithm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//above must be removed for release application to call regular new
//for now all call to new are replaced by a debug version of new
extern CMdProgressDlg mdProgress; //created in algorithm.h to call back_prop() fn
extern network ntwk;//we may no longer need this since ntwk is passed by ref to const for md obj

//END WIN specific
#include "mindist.h"
#include "rand.h"
//figure out which of these we really need in the c++ ver
//#include<stdio.h>
#include<stdlib.h>
#include <math.h>
//#include<ctype.h> 
#include<io.h> 
/*  The minimum disturbance priciple is a method of training a feed forward network of adaptive linear elements.
    The training method is implemented by choosing the nodes which require the least amount of adjustment to
    change the value of their output.The node type for this trainig algorithm must be threshold.  Backprop
    is better suited for training a feed forward network with continuous outputs though.  */
int test;
int *md_num_weights; //used to indirectly pass number of weights to costructor for md_unit_train
extern md *md_ntwk;

md_unit_train::md_unit_train()
{
	//new must call the default constructor when initializing arrays of objects bp_unit_train
	weights=new double[*md_num_weights];
	weights_temp=new double[*md_num_weights];
}

md_unit_train::~md_unit_train()
{
	delete[] weights;
	delete[] weights_temp;
}

md::md(network *ntwk_input)
{
	md_num_weights=new int;
	rand_init();
	//set up the member var's with construction data - make a copy constructor for this later
	input_dim=ntwk_input->input_dim;
	passes=ntwk_input->passes;
	pass_num=0;
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
	global_error=new double[passes];
	data_p=new double[input_dim];
    desired_out=new double[output_dim];
    // allocate rank matrix for ranking nodes according to abs value of linear output 
    rank_mat=new int*[layers];
	for(int i =0;i<layers;i++)
		rank_mat[i]=new int[(layer_p+i)->units];
	//make unit_train items
    md_unit_p=new md_unit_train*[layers];
    for(count_l=0;count_l<layers;count_l++)  
    {
        if(count_l==0)
        {
			*md_num_weights=input_dim+1;//so default constructor for md_unit_train obj sets up correct size array
            md_unit_p[count_l]=new md_unit_train[(layer_p+count_l)->units];
        }
        if(count_l!=0)  // allocate array of weight vectors for each md_unit_train in the remaining layers 
        {
			*md_num_weights=(layer_p+count_l-1)->units + 1;
			md_unit_p[count_l]=new md_unit_train[(layer_p+count_l)->units];
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
                    *((*(md_unit_p+count_l)+count_u)->weights+count_w)= \
						lower_range+rand2()*(upper_range-lower_range);
					//init weights temp
					*((*(md_unit_p+count_l)+count_u)->weights_temp+count_w)=\
						*((*(md_unit_p+count_l)+count_u)->weights+count_w);
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                for(count_w=0;count_w<(layer_p+count_l-1)->units+1;count_w++)
                {
                    *((*(md_unit_p+count_l)+count_u)->weights+count_w)= \
						lower_range+rand2()*(upper_range-lower_range);
					//init weights temp
					*((*(md_unit_p+count_l)+count_u)->weights_temp+count_w)=\
						*((*(md_unit_p+count_l)+count_u)->weights+count_w);
                }
            }
        }
    }
//end setup array of unit-train objects	
}

md::~md()
{
	for(int i=layers-1;i>=0;i--)//delete layers of bp_unit_train
	{
		delete[] md_unit_p[i];
	}
	delete[] md_unit_p;
	for(int i=layers-1;i>=0;i--)
	{
		delete[] rank_mat[i];
	}
	delete[] rank_mat;
	delete[] desired_out;
	delete[] data_p;
	delete[] global_error;
	delete md_num_weights;
	rand_cleanup();//deletes seed created in rand_init
}

 
void md::min_disturbance()
{
    int count_t; 
    output_dim=(layer_p+(layers-1))->units;
    md_file_open();
    for(count_l=0;count_l<layers;count_l++)  // set all unit_trained flags =0 
    {
        for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
        {
            (*(md_unit_p+count_l)+count_u)->trained=0;
        }
    }
    while(pass_num<passes){
		fprintf(fp1,"pass=%d\n",passes);
	//BEGIN WINSPECIFIC
	if(pass_num%5==0)
		mdProgress.m_mdProgressCtrl.SetPos(pass_num);
	//END WINSPECIFIC
	for(adaption_layer=0;adaption_layer<layers;adaption_layer++)
	{
		fprintf(fp1,"\tadaption layer=%d\n",adaption_layer);

	for(count=0;count<num_vectors*2;count++) // for each pass we present 2* the number of training vectors - 	                                              

		//for good coverage     
    { 
		fprintf(fp1,"\t\tvector_count=%d  ",count);

        // we need to present the data_vectors to the training algorithm acyclically - 
		//there are (input_dim+output_dim-1) commas per record 
        vector_selection=(int )(rand2()*num_vectors);  // choose a random number between 0 and num_vectors 
        fseek_to_vector(vector_selection);
        md_get_data(); // puts data from fp into data_p and desired_out
		fprintf(fp1,"\t\tvector_selection=%d   ",vector_selection);
		for(int tt=0;tt<input_dim;tt++)
			fprintf(fp1,"%lg, ",*(data_p+tt));
		for(int tt=0;tt<output_dim;tt++)
			fprintf(fp1,"%lg, ",*(desired_out+tt));
		fprintf(fp1,"\n");
        for(count_t=0;count_t<3;count_t++)   // handles whether we'll be updating single nodes, pairs, or triplets 
        {

            md_propagate(data_p);  // md_propagate the input,  the output is present at both md_unit_p and layer_p 
            error=md_error_comp(desired_out);  //  compute the bipolar error  
            rank();  // rank the nodes according to abs(linear_output)   
            // exit the training loop if the error is zero 
            if(error==0)  
            break;
            switch (count_t)  // update single, pair or triplet of units 
            {
                case 0://updating single units
                {
                    if(error==0)  // checks if we've reduced the error to zero   
                    break;
                    // we rank the nodes after each layer update since changes in the previous layer affect rankings 
					//in the current layer 
                    rank();  
                    if(adaption_layer==0)  //  set the appropriate number of inputs to the node on the current layer 
                    num_inputs=input_dim+1;  // add one for bias since we'll want to adjust this weight also 
                    if(adaption_layer!=0)
                    num_inputs=(layer_p+adaption_layer-1)->units+1;  // number of inputs = units on previous layer + 1 for bias 
                    for(count_u=0;count_u<(layer_p+adaption_layer)->units;count_u++) // step through layer, updating from lowest rank to highest 
                    {
						fprintf(fp1,"count_u=%d *(*(rank_mat+%d)+%d)=%d  ",count_u,adaption_layer,count_u,*(*(rank_mat+adaption_layer)+count_u));    
                        z=*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u);
                        error=md_error_comp(desired_out);
                        if(error==0)
                        break;
                        /* rank_mat[i,j] gives unit number on layer j of ranking i - rank of 0 is unit with smallest abs value of linear output                              */
                        md_update_weight(*(*(rank_mat+adaption_layer)+count_u),num_inputs,data_p);
                        error_new=md_error_comp(desired_out);
                        if(error_new>=error)  // we reject weight change and restore original weights 
                        {
                            if(adaption_layer==0)
                            {
                                for(count_w=0;count_w<=input_dim;count_w++)
                                {
                                    *(z->weights+count_w)=*(z->weights_temp+count_w);
                                }
                            }
                            if(adaption_layer!=0)
                            {
                               for(count_w=0;count_w<=(layer_p+adaption_layer-1)->units;count_w++)
                               {
                                   *(z->weights+count_w)=*(z->weights_temp+count_w);
                               }
                            }
                            md_propagate(data_p);
                       }
                       if(error_new<error) // we accept weight change and set weights_temp to the new values 
                       {
                            if(adaption_layer==0)
                            {
                                for(count_w=0;count_w<=input_dim;count_w++)
                                {
                                    *(z->weights_temp+count_w)=*(z->weights+count_w);
                                }
                            }
                            if(adaption_layer!=0)
                            { 
                                for(count_w=0;count_w<=(layer_p+adaption_layer-1)->units;count_w++)
                                {
                                    *(z->weights_temp+count_w)=*(z->weights+count_w);
                                }
                            }
                            md_propagate(data_p);
                       }// end if 
                    }// end count_u 
					fprintf(fp1,"\n");
                    break;
                }// end case 0 
                
                case 1://updating pairs of units
                {
                    if((layer_p+adaption_layer)->units==1)  // if there is only one unit on layer , we can't update pair, so break 
                    break;
                    if(error==0)   //checks if we've reduced the error to zero   
                    break;
                    rank();  // rank the nodes according to abs(linear_output)  
                    if(adaption_layer==0)  //  set the appropriate number of inputs to the node on the current layer 
                    num_inputs=input_dim+1;  // add one for bias since we'll want to adjust this weight also 
                    if(adaption_layer!=0)
                    num_inputs=(layer_p+adaption_layer-1)->units+1;  // number of inputs = units on previous layer + 1 for bias 
                    // we step through layer in pairs, updating two at a time 
					int number_of_pairs=(layer_p+adaption_layer)->units-1;
					for(count_u=0;count_u<number_of_pairs;count_u++) 
                    {
                        error=md_error_comp(desired_out);
                        if(error==0)
                        break;
                        // rank_mat[i,j] gives unit number on layer j of ranking i - rank of 0 is
                        // unit with smallest abs value of linear output                             
                        md_update_weight(*(*(rank_mat+adaption_layer)+count_u),num_inputs,data_p);
                        // update the unit with the next rank 
                        md_update_weight(*(*(rank_mat+adaption_layer)+count_u+1),num_inputs,data_p);
                        error_new=md_error_comp(desired_out);
                        if(error_new>=error)  // we reject weight change and restore original weights 
                        {
                            if(adaption_layer==0)
                            {
                               for(count_w=0;count_w<=input_dim;count_w++)
                               {
                                    // restore weights for higher ranked unit 
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w)=\
										*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w);
                                   
									// restore weights for unit with next rank 
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w)=\
										*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w);
                               }
                            }
                            if(adaption_layer!=0)
                            {
                                for(count_w=0;count_w<=(layer_p+adaption_layer-1)->units;count_w++)
                                {
                                   *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w)=\
									   *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w);
                                   
								   *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w)=\
									   *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w);
                                }
                            }
                            md_propagate(data_p);
                        }
                        if(error_new<error) // we accept weight change and set weights_temp to the new values 
                        {

                            if(adaption_layer==0)
                            {
								(*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->trained+=1;
								(*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->trained+=1;
                                for(count_w=0;count_w<=input_dim;count_w++)
                                {
                                    // set weights_temp for higher ranked unit 
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w)=\
										*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w);


                                    // set weights_temp for unit with next rank 
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w)=\
										*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w);

                                }
                            }
                            if(adaption_layer!=0)
                            { 
								(*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->trained+=1;
								(*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->trained+=1;
                                for(count_w=0;count_w<=(layer_p+adaption_layer-1)->units;count_w++)
                                {
                                    *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w)=\
										*((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w);

                                    *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w)=\
										*((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w);
                               }
                            }
                            md_propagate(data_p);
                        }// end if 
                        //count_u+=1; //  increment count u so it doesn't start the next pair with the lower ranked unit from the current pair  
                    }//end count_u 
                    break;
                }//end case 1 
                case 2://supposed to be updating triplets - an upgrade for later (not indicated yet that it is needed for this type of training)
                {
                    if((layer_p+adaption_layer)->units==1) // if only one or two units on layer, we can't update triplets, so break 
                    break;
                    if((layer_p+adaption_layer)->units==2)
                    break;
                    //insert code for updating triplets of units 
                    break;
                }//end case 2
                default:
                {
                    AfxMessageBox("Program Error, my fault.Shutting down now...sorry");
                    exit(1);
                }
            }//end switch 

        }//end count_t for one,two,or three node update 
    }//  end adaption vectors count 
    }// end training_layer count 
    
    rewind(fp);

    //code to place the global_error for this training pass into array 
    *(global_error+pass_num)=0;
    for(vector_selection=0;vector_selection<num_vectors;vector_selection++)
    {
        md_get_data();
        md_propagate(data_p);
        *(global_error+pass_num)+=md_error_comp(desired_out);
    }
    rewind(fp);
    pass_num++;}//end while training loop 
	fprintf(fp1,"\nError results for each training vector.");
    for(vector_selection=0;vector_selection<num_vectors;vector_selection++)  
    {
        md_get_data();
        md_propagate(data_p);
        fprintf(fp1,"\ne[%d]=%lg  input=",vector_selection,md_error_comp(desired_out));
        for(count_u=0;count_u<input_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(data_p+count_u));
        }
        fprintf(fp1,"  output=");
        for(count_u=0;count_u<output_dim;count_u++)
        {
			fprintf(fp1,"%lg ",(*(md_unit_p+layers-1)+count_u)->threshold_output);
        }
        fprintf(fp1,"  desired out=");
        for(count_u=0;count_u<output_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(desired_out+count_u));
        }
    }
    fprintf(fp1,"\nGlobal error results for each training pass.");
    for(count=0;count<passes;count++)  
    {
        fprintf(fp1,"\ng_e[%d]=%lg",count,*(global_error+count));
    }
    printf("\nAll Done."); 
    fclose(fp);
	md_output_network();
	fclose(fp1);
}


void md::rank() 
{
    for(count_lr=0;count_lr<layers;count_lr++)
    {
        for(count_ur=0;count_ur<(layer_p+count_lr)->units;count_ur++)  // initialize matrix, remember the entry j is the unit number with j rank 
        {
            *(*(rank_mat+count_lr)+count_ur)=count_ur;
        }
        qsort(*(rank_mat+count_lr),(layer_p+count_lr)->units,sizeof(int),compare);
    }
}


int compare(const void *arg1,const void *arg2)
{
    double z;
    const int *c1=(const int *) arg1;
    const int *c2=(const int *) arg2;    
    z = fabs((*(md_ntwk->md_unit_p+md_ntwk->count_lr)+*c1)->linear_output) - fabs((*(md_ntwk->md_unit_p+md_ntwk->count_lr)+*c2)->linear_output );  // compare the linear outputs 
    
    if(z>0)   // return the correct integer 
    return 1;
    if(z<0)
    return -1;
    
    return 0;    
}


// md_propagates input vector throught the network, stores output in the layer_p structure and the md_unit_p structure  
void md::md_propagate(double *data_p)
{
    double sum;
    int count_l_pr,count_u_pr,count_w_pr;
        for(count_l_pr=0;count_l_pr<layers;count_l_pr++)
        {
            if(count_l_pr==0)
            {
                // go trought the frist layer, applying input vector to each node and computing outputs 
                for(count_u_pr=0;count_u_pr<layer_p->units;count_u_pr++)
                {
                    x=*(md_unit_p+count_l_pr)+count_u_pr;
                    sum=(layer_p)->bias * *(x->weights);  // compute bias term 
                     /*tricky - we have to remember that we're using count_w_pr to step through the units on the pervios layer
                       which start at 0, but we're multiplying by non-bias weights which start at 1, so we subtract off 1 
                       from the count in the first term of the sum  */
                    for(count_w_pr=1;count_w_pr<=input_dim;count_w_pr++)
                        sum+=*(data_p+count_w_pr-1) * *(x->weights+count_w_pr);
                    if(sum>=0)  // output of threshold is sgn(linear output) 
                        temp=1;
                    if(sum<0)
                        temp=-1;
                    x->threshold_output=temp;    // store both output types in md_unit_p struct 
                    x->linear_output=sum;
                }// end unit count for layer 0 
            }// end if for layer 0 
            if(count_l_pr!=0)
            {
                for(count_u_pr=0;count_u_pr<(layer_p+count_l_pr)->units;count_u_pr++)
                {
                    x=*(md_unit_p+count_l_pr)+count_u_pr;
                    sum=(layer_p+count_l_pr)->bias * *(x->weights);  // bias term 
                    for(count_w_pr=1;count_w_pr<=(layer_p+count_l_pr-1)->units;count_w_pr++)
                        sum+=(*(md_unit_p+count_l_pr-1)+count_w_pr-1)->threshold_output * *(x->weights+count_w_pr);
                    // bipolar output - the code could easily be converted, or modified to accomodate binary threshold functions 
                    // just change -1 below to 0 
                    if(sum>=0)  // output of threshold is sgn(linear_output) 
                        temp=1;
                    if(sum<0)
                        temp=-1;    
                    x->threshold_output=temp;
                    x->linear_output=sum;
                }// end unit count for layer count_l_pr 
            }// end if for layer !=0 
        }//  end layers count  
}    

/*computes the binary error at the output layer,  returns type double since we allow different activation functions
  for this module - even though it is intended for use with threshold activation.  This fn should always be preceeded
  by a call to propagate()  */
double md::md_error_comp(double *desired_out)  
{
    int count_u_e;
    double temp_e;
    temp_e=0;
    for(count_u_e=0;count_u_e<output_dim;count_u_e++)
    {
        if(*(desired_out+count_u_e)!=(*(md_unit_p+(layers -1))+count_u_e)->threshold_output)
       temp_e+=1;
    }
    return temp_e;
}

void md::md_update_weight(int update_unit,int num_inputs,double *data_p)
{
    int input_mod,not_changed=1;
	test=1;
    double old_linear,direction=1,increment;
    /*we have to be careful when adjusting weights tied to zero input.  For the greatest generality,
      we have placed no restrictions on the types of values that mae be used as input.  If we try and
      adjust the weight that is tied to an input of 0 for a particular input vector, we don't ever
      exit the loop where we try and change the output.  Similar for bias weight when bias is 0. */
    while(test>0)
    {    
        input_mod=(int )(rand2()*num_inputs);
        if(input_mod==0 && (layer_p+adaption_layer)->bias==0)  // check that we're not trying to adjust bias weight for zero bias 
        {
            test++;
            continue;
        }
		//for a zero input, the output of the node will never change if we adapt the weight tied to that input	
        if(adaption_layer==0 && input_mod!=0)  // we need to check the data tied to the input weight 
        {
            if(*(data_p+input_mod-1)==0)  // remembering input_mod runs from 0..input_dim, and data_p from 0..input_dim-1 
            {
                test++;
                continue;
            }
        }
        if(test>10000)  // handles the case where we can't find a weight tied to a non-zero value 
        {
            CMdZeroInputwarningDlg dlg;
			dlg.DoModal();

            (*(md_unit_p+adaption_layer)+update_unit)->trained=1;  // we don't train the unit in this case 
            return;
        }
        // if we've made it this far, then the chosen weight is not tied to zero input 
        test=-1;
    }   
 
    increment=training_param*direction;
    
    *((*(md_unit_p+adaption_layer)+update_unit)->weights+input_mod)+=increment;// SUSPECT - WE MAY NEED TO UNDO 
    
    old_linear=(*(md_unit_p+adaption_layer)+update_unit)->linear_output;
                            
    md_propagate(data_p);

    // make sure weight changes are heading in the proper direction 
    if(fabs(old_linear)<fabs((*(md_unit_p+adaption_layer)+update_unit)->linear_output))
    {
        direction=-1;
        
        
        increment=training_param*direction;
    }

    /* if we have flipped an output with the test to see if the weight change was going in the proper direction 
       we need to accept the weight change of increment and stop adapting the current node  */
    if((old_linear*(*(md_unit_p+adaption_layer)+update_unit)->linear_output)<0)
    {
        not_changed=-1;
    }
    
    test=0;
    while(not_changed>0)  // change the bipolar output of update node 
    {
        test++;
        
        *((*(md_unit_p+adaption_layer)+update_unit)->weights+input_mod)+=increment;
		
        md_propagate(data_p);

        if((old_linear * (*(md_unit_p+adaption_layer)+update_unit)->linear_output)<0)
        not_changed=-1;
		
        if(test>10000)  // test for excessive iterations 
        {
            AfxMessageBox("ERROR: excessive iterations in weight update loop.");
            exit(1);
            
        }
    }
    (*(md_unit_p+adaption_layer)+update_unit)->trained+=1;  // increment unit trained flag 
}


// This prints trained network to file specified by FILE *fp1 
void md::md_output_network()
{
    int count_l_p,count_u_p,count_w_p;
    CTime t=CTime::GetCurrentTime();
	CString s=t.Format("%A, %B, %d, %Y");
	fprintf(fp1,"Minimum Disturbance Training Run \n[%s]",s);
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
                    fprintf(fp1,"=:%lg\n",*((*(md_unit_p+count_l_p)+count_u_p)->weights+count_w_p));
                }
                fprintf(fp1,"\nunit trained =\t%d",(*(md_unit_p)+count_u_p)->trained);                
            }
        }
        if(count_l_p!=0)
        {
            for(count_u_p=0;count_u_p<(layer_p+count_l_p)->units;count_u_p++)
            {
                fprintf(fp1,"\nweights   UINT[%d][%d]\n",count_l_p,count_u_p);
                for(count_w_p=0;count_w_p<=(layer_p+count_l_p-1)->units;count_w_p++)
                {
                    fprintf(fp1,"=:%lg\n",*((*(md_unit_p+count_l_p)+count_u_p)->weights+count_w_p));
                }
                fprintf(fp1,"unit trained =\t%d\n",(*(md_unit_p+count_l_p)+count_u_p)->trained);
                
            }
        }
    }
	fprintf(fp1,"\n\n");
}


void md::md_get_data(void)
{
    for(count_w=0;count_w<input_dim;count_w++)  //  get training vector 
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
    for(count_w=0;count_w<output_dim;count_w++)  //  get output vector vector 
    {
        fscanf(fp,"%lg",(desired_out+count_w));
        if( (*(desired_out+count_w)!=(double)1.0) && (*(desired_out+count_w)!=(double)-1.0) )
        {
			AfxMessageBox("ERROR: output for madaline with minimum disturbance training is bipolar. Expecting 1,or -1 in data file for desired output");
           //we need to throw some exceptions to get back to where we belong! md_ntwk->~md();//clean up
			//mdProgress.~CMdProgressDlg();//get out
        }
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
    // we now neet to verify that we're at the end of a record,  or at least scan whitespace until data is encountered 
    buf[0]='\n';
    while(isspace(buf[0]))
    {
        save_position=ftell(fp);
        buf[0]=fgetc(fp);
        if( (vector_selection!=num_vectors-1) && (feof(fp)) ) /* check that we have not reached EOF prematurely */
        {
            AfxMessageBox("\nERROR: unexpected EOF in data file");
            exit(1);
        }
        if(buf[0]==',') /* check that we are indeed at the end of a record */
        {
            AfxMessageBox("ERROR: unexpected field delimiter - expection end of record");
            exit(1);
        }
   }
   fseek(fp,save_position,SEEK_SET); /* put fp back one since data was encounterd on last read and we're set to scan next vector */
}




void md::md_file_open(void)  /*  opens training data file */
{
	//windows specific
    while((fp=fopen(input_file_buf,"r+"))==NULL)
    {
        AfxMessageBox("Error opening data file");
        exit(1);
    }
	while((fp1=fopen(output_file_buf,"a+"))==NULL)
    {
        AfxMessageBox("Error opening data file");
        exit(1);
    }

    
}

void md::fseek_to_vector(int selection)
{
		int commas_found=0;
        // The following puts fp at the proper location to scan in the randomly chosen data vector     
        rewind(fp);// set fp to beginning of file 
        while(commas_found<selection*(input_dim+output_dim-1)) // scan past the proper number of commas 
        {
            buf[0]=fgetc(fp);
            if(buf[0]==',')
            commas_found+=1;
            if(feof(fp))
            {
                AfxMessageBox("ERROR: unexpected end of file.");
                exit(1);

            }
        }
        //  scan past any whitespace 
        buf[0]='\n';
        while(isspace(buf[0]))
        {
            save_position=ftell(fp);
            buf[0]=fgetc(fp);
        }
        fseek(fp,save_position,SEEK_SET); // put fp back one since data was encounterd on last read 
        if(selection!=0) // check if there is a preceeding data_vector 
        fscanf(fp,"%lg",&temp);    // scan in last data value from record selection-1 
        //  scan past any whitespace 
        buf[0]='\n';
        while(isspace(buf[0]))
        {
            save_position=ftell(fp);
            buf[0]=fgetc(fp);
            if( (selection!=num_vectors-1) && (feof(fp)) ) // check that we have not reached EOF prematurley 
            {
                AfxMessageBox("ERROR: unexpected EOF in data file");
                exit(1);

            }
            if(buf[0]==',') // check that we are indeed at the end of a record 
            {
                AfxMessageBox("ERROR: unexpected field delimiter - expecting end of record");
                exit(1);
            }
        }
        fseek(fp,save_position,SEEK_SET); // put fp back one since data was encounterd on last read and we're set to scan next vector 
        // now fp should point to the proper location  
}