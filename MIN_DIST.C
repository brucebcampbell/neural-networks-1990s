#include<stdio.h>
#include<stdlib.h>
#include "min_dist.h"
#include "neural.h"
#include "rand.h"
#include <math.h>
#include<ctype.h> 
#include<io.h> 

/*  The minimum disturbance priciple is a method of training a feed forward network of adaptive linear elements.
    The training method is implemented by choosing the nodes which require the least amount of adjustment to
    change the value of their output.The node type for this trainig algorithm must be threshold.  Backprop
    is better suited for training a feed forward network with continuous outputs though.  */



struct unit_train{  /* data is loaded into matrix of unit_train structures for training, trained network is saved back to layer structure */
    int trained;                
    double *weights;
    double *weights_temp;    
    double linear_output;
    double threshold_output;
    int rank;};    /* rank member is not used at this point */
struct unit_train **md_unit_p;

extern long int *seed;

struct unit_train *z,*x;  /* optimization variables to be used in loops where layer and unit are fixed and we're stepping through weights */
                           /* z is used in min_disturbance(), x in propagate()  */

static FILE *fp, *fp1;       
static char buf[80];
long int save_position;

double *data_p,*desired_out;  /* input & output vectors */

int pass_num=0;  /* counter for the number of training passes for the training data */

int vector_number;  /* indicates the random choice of training vector selected from file, used in min_disturbance() and md_get_data() */

int count,count_l,count_u,count_w;  /* counters for stepping through training vectors, layers, units, weights */

int adaption_layer; /* indicates the layer nodes are being adapted on */

int count_lr,count_ur;  /* used by rank and compare functions */

double error,error_new,temp;  /* used in md_update_weights() and min_disturbance() */

double *global_error; /* an array of ptrs to error sums, indexed over training passes */

int output_dim;

int **rank_mat;

int update_unit,num_inputs;  /* used in min_disturbance(), md_update_weight() */

  
void min_disturbance(struct layer *layer_p,struct network ntwk_info)
{
    int count_t,commas_found; 
    output_dim=(layer_p+(ntwk_info.layers-1))->units;

    if((global_error=(double *)malloc(ntwk_info.passes*sizeof(double)))==NULL)
    {
        fprintf(stderr,"\nERROR: allocating array of pointer to global error");
        fprintf(stderr,"\nPress any key to exit.");
        gets(buf);
        exit(1);
    }

    md_file_open();
    
    md_unit_p_init(layer_p);

    /* for now we are not using *->trained anywhere */
    for(count_l=0;count_l<ntwk_info.layers;count_l++)  /* set all unit_trained flags =0 */
    {
        for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
        {
            (*(md_unit_p+count_l)+count_u)->trained=0;
        }
    }

    
    while(pass_num<ntwk_info.passes){
    
    for(adaption_layer=0;adaption_layer<ntwk_info.layers;adaption_layer++)
    {
                         
    for(count=0;count<ntwk_info.num_vectors*2;count++) /* for each pass we present 2* the number of training vectors - for good coverage */
    {
        /* we need to present the data_vectors to the training algorithm acyclically - there are (input_dim+output_dim-1) commas per record */
        vector_number=(int )(rand2()*ntwk_info.num_vectors);  /* choose a random number between 0 and num_vectors */
        commas_found=0;


        /* The following puts fp at the proper location to scan in the randomly chosen data vector     */
        rewind(fp);/* set fp to beginning of file */
        while(commas_found<vector_number*(ntwk_info.input_dim+output_dim-1)) /* scan past the proper number of commas */
        {
            buf[0]=fgetc(fp);
            if(buf[0]==',')
            commas_found+=1;
            if(feof(fp))
            {
                fprintf(stderr,"\nERROR: unexpected end of file.");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);

            }
        }
        /*  scan past any whitespace */
        buf[0]='\n';
        while(isspace(buf[0]))
        {
            save_position=ftell(fp);
            buf[0]=fgetc(fp);
        }
        fseek(fp,save_position,SEEK_SET); /* put fp back one since data was encounterd on last read */
        if(vector_number!=0) /* check if there is a preceeding data_vector */
        fscanf(fp,"%lg",&temp);    /* scan in last data value from record vector_number-1 */
        /*  scan past any whitespace */
        buf[0]='\n';
        while(isspace(buf[0]))
        {
            save_position=ftell(fp);
            buf[0]=fgetc(fp);
            if( (vector_number!=ntwk_info.num_vectors-1) && (feof(fp)) ) /* check that we have not reached EOF prematurley */
            {
                fprintf(stderr,"\nERROR: unexpected EOF in data file");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);

            }
            if(buf[0]==',') /* check that we are indeed at the end of a record */
            {
                fprintf(stderr,"\nERROR: unexpected field delimiter - expecting end of record");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }
        fseek(fp,save_position,SEEK_SET); /* put fp back one since data was encounterd on last read and we're set to scan next vector */
        /* now fp should point to the proper location  */

            
        md_get_data(); /* puts data from fp into data_p and desired_out  */
                 
        for(count_t=0;count_t<3;count_t++)   /* handles whether we'll be updating single nodes, pairs, or triplets */
        {
            md_propagate(layer_p,data_p);  /* md_propagate the input,  the output is present at both md_unit_p and layer_p */

            error=md_error_comp(layer_p,desired_out);  /*  compute the bipolar error  */
            
            rank(layer_p);  /* rank the nodes according to abs(linear_output)   */
            
            /* exit the training loop if the error is zero */
            if(error==0)  
            break;

            switch (count_t)  /* update single, pair or triplet of units */
            {
                case 0:
                {
                    if(error==0)  /* checks if we've reduced the error to zero   */
                    break;
                    /* we rank the nodes after each layer update since changes in the previous layer affect rankings in the current layer */
                    rank(layer_p);  
                        
                    if(adaption_layer==0)  /*  set the appropriate number of inputs to the node on the current layer */
                    num_inputs=ntwk_info.input_dim+1;  /* add one for bias since we'll want to adjust this weight also */
                    if(adaption_layer!=0)
                    num_inputs=(layer_p+adaption_layer-1)->units+1;  /* number of inputs = units on previous layer + 1 for bias */
                    for(count_u=0;count_u<(layer_p+adaption_layer)->units;count_u++) /* step through layer, updating from lowest rank to highest */
                    {
                        z=*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u);
                         
                        error=md_error_comp(layer_p,desired_out);
                        if(error==0)
                        break;
                        
                        /* rank_mat[i,j] gives unit number on layer j of ranking i - rank of 0 is unit with smallest abs value of linear output                              */
                        md_update_weight(layer_p,*(*(rank_mat+adaption_layer)+count_u),num_inputs,data_p);
                         
                        error_new=md_error_comp(layer_p,desired_out);
                        
                        if(error_new>=error)  /* we reject weight change and restore original weights */
                        {
                            if(adaption_layer==0)
                            {
                                for(count_w=0;count_w<=ntwk_info.input_dim;count_w++)
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
                            md_propagate(layer_p,data_p);
                       }
                       if(error_new<error) /* we accept weight chnage and set weights_temp to the new values */
                       {
                            if(adaption_layer==0)
                            {
                                for(count_w=0;count_w<=ntwk_info.input_dim;count_w++)
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
                            md_propagate(layer_p,data_p);
                       }/* end if */
                    }/* end count_u */
                    
                    break;
                }/* end case 0 */
                
                case 1:
                {
                    if((layer_p+adaption_layer)->units==1)  /* if there is only one unit on layer , we can't update pair, so break */
                    break;

                    if(error==0)  /* checks if we've reduced the error to zero   */
                    break;
                     
                    rank(layer_p);  /* rank the nodes according to abs(linear_output)  */ 
                    if(adaption_layer==0)  /*  set the appropriate number of inputs to the node on the current layer */
                    num_inputs=ntwk_info.input_dim+1;  /* add one for bias since we'll want to adjust this weight also */
                    if(adaption_layer!=0)
                    num_inputs=(layer_p+adaption_layer-1)->units+1;  /* number of inputs = units on previous layer + 1 for bias */

                    /* we step through layer in pairs, updating two at a time */
                    for(count_u=0;count_u<((layer_p+adaption_layer)->units-(layer_p+adaption_layer)->units%2);count_u++) 
                    {
                        error=md_error_comp(layer_p,desired_out);
                        if(error==0)
                        break;
                        
                        /* rank_mat[i,j] gives unit number on layer j of ranking i - rank of 0 is
                           unit with smallest abs value of linear output  */                            
                        md_update_weight(layer_p,*(*(rank_mat+adaption_layer)+count_u),num_inputs,data_p);
                        /* update the unit with the next rank */
                        md_update_weight(layer_p,*(*(rank_mat+adaption_layer)+count_u+1),num_inputs,data_p);
                         
                        error_new=md_error_comp(layer_p,desired_out);
                         
                        if(error_new>=error)  /* we reject weight change and restore original weights */
                        {
                            if(adaption_layer==0)
                            {
                               for(count_w=0;count_w<=ntwk_info.input_dim;count_w++)
                               {
                                    /* restore weights for higher ranked unit */
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w)=*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w);
                                    /* restore weights for unit with next rank */
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w)=*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w);
                               }
                            }
                            if(adaption_layer!=0)
                            {
                                for(count_w=0;count_w<=(layer_p+adaption_layer-1)->units;count_w++)
                                {
                                   *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w)=*((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w);
                                   *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w)=*((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w);
                                }
                            }
                            md_propagate(layer_p,data_p);
                        }
                        if(error_new<error) /* we accept weight chnage and set weights_temp to the new values */
                        {
                            if(adaption_layer==0)
                            {
                                for(count_w=0;count_w<=ntwk_info.input_dim;count_w++)
                                {
                                    /* set weights_temp for higher ranked unit */
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w)=*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w);
                                    /* set weights_temp for unit with next rank */
                                    *((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w)=*((*(md_unit_p)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w);
                                }
                            }
                            if(adaption_layer!=0)
                            { 
                                for(count_w=0;count_w<=(layer_p+adaption_layer-1)->units;count_w++)
                                {
                                    *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights_temp+count_w)=*((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u))->weights+count_w);
                                    *((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights_temp+count_w)=*((*(md_unit_p+adaption_layer)+*(*(rank_mat+adaption_layer)+count_u+1))->weights+count_w);
                               }
                            }
                            md_propagate(layer_p,data_p);
                                
                        }/* end if */
                        count_u+=1; /*  increment count u so it doesn't start the next pair with the lower ranked unit from the current pair  */
                             
                    }/* end count_u */

                    break;
                }/* end case 1 */
                    
                case 2:
                {
                    if((layer_p+adaption_layer)->units==1) /* if only one or two units on layer, we can't update triplets, so break */
                    break;
                    if((layer_p+adaption_layer)->units==2)
                    break;

                    
                    /* insert code for updating triplets of units */

                        
                    break;
                }/* end case 2 */
                
                default:
                {
                    fprintf(stderr,"\nError in switch statement in weight update code.");
                    fprintf(stderr,"\nPress any key to exit.");
                    gets(buf);
                    exit(1);
                }
            }/* end switch */

        }/* end count_t for one,two,or three node update */
    }/*  end training vectors count */
    }/* end adaption_layer count */
    
    rewind(fp);

    /* code to place the global_error for this training pass into array */
    *(global_error+pass_num)=0;
    for(vector_number=0;vector_number<ntwk_info.num_vectors;vector_number++)
    {
        md_get_data();
        md_propagate(layer_p,data_p);
        *(global_error+pass_num)+=md_error_comp(layer_p,desired_out);
    }
    rewind(fp);
         
    pass_num++;}/* end while training loop */
    
    /* send network to file   */
    md_output_network(layer_p);

    /* send error results to file */
    fprintf(fp1,"\nError results for each training vector.");
    for(vector_number=0;vector_number<ntwk_info.num_vectors;vector_number++)  
    {
        md_get_data();
        md_propagate(layer_p,data_p);
        fprintf(fp1,"\ne[%d]=:%lg  input=:",vector_number,md_error_comp(layer_p,desired_out));
        for(count_u=0;count_u<ntwk_info.input_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(data_p+count_u));
        }
        fprintf(fp1,"  output=:");
        for(count_u=0;count_u<output_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*((layer_p+ntwk_info.layers-1)->output+count_u));
        }
        fprintf(fp1,"  desired out=:");
        for(count_u=0;count_u<output_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(desired_out+count_u));
        }
    }

    /* send global error results to file */
    fprintf(fp1,"\nGlobal error results for each training pass.");
    for(count=0;count<ntwk_info.passes;count++)  /* output the global error for each training pass */
    {
        fprintf(fp1,"\ng_e[%d]=:%lg",count,*(global_error+count));
    }
    printf("\nAll Done."); 
    fclose(fp);
    fclose(fp1);
}




static void rank(struct layer *layer_p) 
{
    for(count_lr=0;count_lr<ntwk_info.layers;count_lr++)
    {
        for(count_ur=0;count_ur<(layer_p+count_lr)->units;count_ur++)  /* initialize matrix, remember the entry j is the unit number with j rank */
        {
            *(*(rank_mat+count_lr)+count_ur)=count_ur;
        }
        qsort(*(rank_mat+count_lr),(layer_p+count_lr)->units,sizeof(int),compare);
    }
}


static int compare(const void *arg1,const void *arg2)
{
    double z;
    const int *c1=(const int *) arg1;
    const int *c2=(const int *) arg2;    
    z = fabs((*(md_unit_p+count_lr)+*c1)->linear_output) - fabs((*(md_unit_p+count_lr)+*c2)->linear_output );  /* compare the linear outputs */
    
    if(z>0)   /* return the correct integer */
    return 1;
    if(z<0)
    return -1;
    
    return 0;    
}


/* md_propagates input vector throught the network, stores output in the layer_p structure and the md_unit_p structure  */
static void md_propagate(struct layer *layer_p,double *data_p)
{
    double sum;
    int count_l_pr,count_u_pr,count_w_pr;
        for(count_l_pr=0;count_l_pr<ntwk_info.layers;count_l_pr++)
        {
            if(count_l_pr==0)
            {
                /* go trought the frist layer, applying input vector to each node and computing outputs */
                for(count_u_pr=0;count_u_pr<layer_p->units;count_u_pr++)
                {
                    x=*(md_unit_p+count_l_pr)+count_u_pr;
                    sum=(layer_p)->bias * *(x->weights);  /* compute bias term */

                     /*tricky - we have to remember that we're using count_w_pr to step through the units on the pervios layer
                       which start at 0, but we're multiplying by non-bias weights which start at 1, so we subtract off 1 
                       from the count in the first term of the sum  */
                    
                    for(count_w_pr=1;count_w_pr<=ntwk_info.input_dim;count_w_pr++)
                        sum+=*(data_p+count_w_pr-1) * *(x->weights+count_w_pr);
                        
                    if(sum>=0)  /* output of threshold is sgn(linear output) */
                        temp=1;
                    if(sum<0)
                        temp=-1;
                        
                    x->threshold_output=temp;    /* store both output types in md_unit_p struct */
                    x->linear_output=sum;
                    
                    switch(layer_p->node_type)  /* we could move this to be done once on exit of min_dist to save time */
                    {
                        case THRESHOLD:
                        {
                            *(layer_p->output+count_u_pr)=x->threshold_output;
                            break;
                        }
                        case LINEAR:
                        {
                            *(layer_p->output+count_u_pr)=x->linear_output;
                            break;
                        }
                        default:
                        {
                            fprintf(stderr,"\nERROR: did not recgonize node_type presented to min_disturbance training.");
                            fprintf(stderr,"\nPress any key to exit.");
                            gets(buf);
                            exit(1);
                        }
                    }/* end switch */
                }/* end unit count for layer 0 */
            }/* end if for layer 0 */
            
            if(count_l_pr!=0)
            {
                for(count_u_pr=0;count_u_pr<(layer_p+count_l_pr)->units;count_u_pr++)
                {
                    x=*(md_unit_p+count_l_pr)+count_u_pr;
                    sum=(layer_p+count_l_pr)->bias * *(x->weights);  /* bias term */
                    
                    for(count_w_pr=1;count_w_pr<=(layer_p+count_l_pr-1)->units;count_w_pr++)
                        sum+=(*(md_unit_p+count_l_pr-1)+count_w_pr-1)->threshold_output * *(x->weights+count_w_pr);


                    /* bipolar output - the code could easily be converted, or modified to accomodate binary threshold functions */
                    /* just change -1 below to 0 */
                    if(sum>=0)  /* output of threshold is sgn(linear_output) */
                        temp=1;
                    if(sum<0)
                        temp=-1;
                        
                    x->threshold_output=temp;
                    x->linear_output=sum;
                    
                    switch((layer_p+count_l_pr)->node_type)
                    {
                        case THRESHOLD:
                        {
                            *((layer_p+count_l_pr)->output+count_u_pr)=x->threshold_output;
                            break;
                        }
                        case LINEAR:
                        {
                            *((layer_p+count_l_pr)->output+count_u_pr)=x->linear_output;                            
                        }
                        default:
                        {
                            fprintf(stderr,"\nERROR: did not recognise node_type presented to min_disturbance training.");
                            fprintf(stderr,"\nPress any key to exit.");
                            gets(buf);
                            exit(1);
                        }
                    }/* end switch */
                }/* end unit count for layer count_l_pr */
            }/* end if for layer !=0 */
        }/*  end layers count  */
}    

/*computes the binary error at the output layer,  returns type double since we allow different activation functions
  for this module - even though it is intended for use with threshold activation.  This fn should always be preceeded
  by a call to propagate()  */
static double md_error_comp(struct layer *layer_p,double *desired_out)  
{
    int count_u_e;
    double temp_e;
    temp_e=0;
    for(count_u_e=0;count_u_e<output_dim;count_u_e++)
    {
        if(*(desired_out+count_u_e)!=(*(md_unit_p+(ntwk_info.layers -1))+count_u_e)->threshold_output)
       temp_e+=1;
    }

    return temp_e;
}

static void md_update_weight(struct layer *layer_p,int update_unit,int num_inputs,double *data_p)
{
    int input_mod,not_changed=1,test=1;
    double old_linear,direction=1,increment;
    
    /*we have to be careful when adjusting weights tied to zero input.  For the greatest generality,
      we have placed no restrictions on the types of values that mae be used as input.  If we try and
      adjust the weight that is tied to an input of 0 for a particular input vector, we don't ever
      exit the loop where we try and change the output.  Similar for bias weight when bias is 0. */
      
    while(test>0)
    {    
        input_mod=(int )(rand2()*num_inputs);
        
        if(input_mod==0 && (layer_p+adaption_layer)->bias==0)  /* check that we're not trying tho adjust bias weight for zero bias */
        {
            test++;
            continue;
        }
        if(adaption_layer==0 && input_mod!=0)  /* we need to check the data tied to the input weight */
        {
            if(*(data_p+input_mod-1)==0)  /* remembering input_mod runs from 0..input_dim, and data_p from 0..input_dim-1 */
            {
                test++;
                continue;
            }
        }
        if(test>10000)  /* handles the case where we can't find a weight tied to a non-zero value */
        {
            fprintf(stderr,"POSSIBLE ERROR: you have either won the computational lottery, or tried to present a node");
            fprintf(stderr,"\nwith all zeros, including the bias.  If this is true, enter a positive number:");
            fprintf(stderr,"\notherwise enter a negative number:");
            scanf("%d",&test);
            if(test<0)
                exit(1);

            (*(md_unit_p+adaption_layer)+update_unit)->trained=1;  /* we don't train the unit in this case */
            return;
        }
        /* if we've made it this far, then the chosen weight is not tied to zero input */
        test=-1;
    }   
 
    increment=ntwk_info.training_param*direction;
    
    *((*(md_unit_p+adaption_layer)+update_unit)->weights+input_mod)+=increment;/* SUSPECT - WE MAY NEED TO UNDO */ 
    
    old_linear=(*(md_unit_p+adaption_layer)+update_unit)->linear_output;
                            
    md_propagate(layer_p,data_p);

    /* make sure weight changes are heading in the proper direction */
    if(fabs(old_linear)<fabs((*(md_unit_p+adaption_layer)+update_unit)->linear_output))
    {
        direction=-1;
        
        
        increment=ntwk_info.training_param*direction;
    }

    /* if we have flipped an output with the test to see if the weight change was going in the proper direction 
       we need to accept the weight change of increment and stop adapting the current node  */
    if((old_linear*(*(md_unit_p+adaption_layer)+update_unit)->linear_output)<0)
    {
        not_changed=-1;
    }
    
    test=0;
    while(not_changed>0)  /* change the bipolar output of update node */
    {
        test++;
        
        *((*(md_unit_p+adaption_layer)+update_unit)->weights+input_mod)+=increment;
        md_propagate(layer_p,data_p);

        if((old_linear * (*(md_unit_p+adaption_layer)+update_unit)->linear_output)<0)
        not_changed=-1;

        if(test>10000)  /* test for excessive iterations */
        {
            fprintf(stderr,"\nPOSSIBLE ERROR: excessive iterations in weight update loop.");
            fprintf(stderr,"\nnetwork state will be dumped into output file for examination"); 
            printf("\nEnter a positive numer to exit program, negative to continue:");
            scanf("%d",&test);
            if(test>0)
            {
                fprintf(fp1,"\nERROR DUMP:");
                md_output_network(layer_p);
                fprintf(fp1,"\nupdate pass_number=%d [layer,unit,input_mod]=[%d,%d,%d]",pass_num,adaption_layer,update_unit,input_mod);
                fprintf(fp1,"\nincrement=%lg old_output=%lg present_output=%lg",increment,old_linear,(*(md_unit_p+adaption_layer)+update_unit)->linear_output);
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }
    }

        
    (*(md_unit_p+adaption_layer)+update_unit)->trained+=1;  /* increment unit trained flag */
}


/* This prints trained network to file specified by FILE *fp1 */
static void md_output_network(struct layer *layer_p)
{
    int count_l_p,count_u_p,count_w_p;
    
    for(count_l_p=0;count_l_p<ntwk_info.layers;count_l_p++)  /* write the unit_train structure */
    {
        fprintf(fp1,"\n\t\t\tLAYER [%d]",count_l_p);
        if(count_l_p==0)
        {
            for(count_u_p=0;count_u_p<layer_p->units;count_u_p++)
            {
                fprintf(fp1,"\nweights   UINT[%d][%d]\n",count_l_p,count_u_p);
                for(count_w_p=0;count_w_p<=ntwk_info.input_dim;count_w_p++)
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
}


static void md_get_data(void)
{
    for(count_w=0;count_w<ntwk_info.input_dim;count_w++)  /*  get training vector */
    {
        fscanf(fp,"%lg",(data_p+count_w));
        while(!feof(fp)) 
        {
            buf[0]=fgetc(fp);
            if(buf[0]==',')
            break;
            if(feof(fp))
            {
                fprintf(stderr,"\nERROR: unexpected end of file.");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }            
     }
    for(count_w=0;count_w<output_dim;count_w++)  /*  get output vector vector */
    {
        fscanf(fp,"%lg",(desired_out+count_w));
        if( (*(desired_out+count_w)!=(double)1.0) && (*(desired_out+count_w)!=(double)-1.0) )
        {
            fprintf(stderr,"\nERROR: output for madaline with minimum disturbance training is bipolar.  Expecting 1,or -1");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
        while(!feof(fp) && count_w!=output_dim-1) 
        {
            buf[0]=fgetc(fp);
            if(buf[0]==',')
            break;
            if(feof(fp))
            {
                fprintf(stderr,"\nERROR: unexpected end of file.");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }            
    }
    /* we now neet to verify that we're at the end of a record,  or at least scan whitespace until data is encountered */
    buf[0]='\n';
    while(isspace(buf[0]))
    {
        save_position=ftell(fp);
        buf[0]=fgetc(fp);
        if( (vector_number!=ntwk_info.num_vectors-1) && (feof(fp)) ) /* check that we have not reached EOF prematurely */
        {
            fprintf(stderr,"\nERROR: unexpected EOF in data file");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
        if(buf[0]==',') /* check that we are indeed at the end of a record */
        {
            fprintf(stderr,"\nERROR: unexpected field delimiter - expection end of record");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
   }
   fseek(fp,save_position,SEEK_SET); /* put fp back one since data was encounterd on last read and we're set to scan next vector */
}

static void md_unit_p_init(struct layer *layer_p)
{

    /*  there should also be a cleanup function that frees all of md_unit_p space allocated   */
    if((data_p=malloc(ntwk_info.input_dim*sizeof(double)))==NULL)  /* allocate space for data vector */
        fprintf(stderr,"\nerror allocating data_p");
    if((desired_out=malloc(output_dim*sizeof(double)))==NULL)  /* allocate space for desired output vector */
        fprintf(stderr,"\nerror allocating desired_out");
                       
    if((md_unit_p=malloc(ntwk_info.layers*sizeof(long)))==NULL)  /* allocate array of pointers to array of pointers to units */
        fprintf(stderr,"\nerror allocating md_unit_p"); 
    for(count_l=0;count_l<ntwk_info.layers;count_l++)  /* allocate vectors of struct unit_train corresponding to layers */
    {
        if((*(md_unit_p+count_l)=(struct unit_train *)malloc((layer_p+count_l)->units*sizeof(struct unit_train)))==NULL)
            fprintf(stderr,"\nerror allocating row unit_train");
    }

    for(count_l=0;count_l<ntwk_info.layers;count_l++)  /* alocate space for weights remembering that bias weight is always the first one */
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++) /* allocate array of weight vectors for each unit_train in input layer */
            {
                if(((*(md_unit_p+count_l)+count_u)->weights=(double *)malloc((ntwk_info.input_dim+1)*sizeof(double)))==NULL)
                    fprintf(stderr,"\nerror allocating weights ");
                if(((*(md_unit_p+count_l)+count_u)->weights_temp=(double *)malloc((ntwk_info.input_dim+1)*sizeof(double)))==NULL)
                    fprintf(stderr,"\nerror allocating weights_temp layer 0");
                    
            }
        }
        if(count_l!=0)  /* allocate array of weight vectors for each unit_train in the remaining layers */
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                if(((*(md_unit_p+count_l)+count_u)->weights=(double *)malloc(((layer_p+count_l-1)->units+1)*sizeof(double)))==NULL)
                    fprintf(stderr,"\nerror allocating weights layer !=0");
                if(((*(md_unit_p+count_l)+count_u)->weights_temp=(double *)malloc(((layer_p+count_l-1)->units+1)*sizeof(double)))==NULL)
                    fprintf(stderr,"\nerror allocating weights layer !=0");
                    
            }
        }
    }
    /*  initialize the weights for matrix of struct unit_train */
    for(count_l=0;count_l<ntwk_info.layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                (*(md_unit_p)+count_u)->trained=0;
                for(count_w=0;count_w<ntwk_info.input_dim+1;count_w++)
                {
                    *((*(md_unit_p)+count_u)->weights+count_w)=*((*(layer_p->weights+count_u))+count_w);
                    *((*(md_unit_p)+count_u)->weights_temp+count_w)=*((*(layer_p->weights+count_u))+count_w); 
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                (*(md_unit_p+count_l)+count_u)->trained=0;
                for(count_w=0;count_w<(layer_p+count_l-1)->units+1;count_w++)
                {
                    *((*(md_unit_p+count_l)+count_u)->weights+count_w)=*((*((layer_p+count_l)->weights+count_u))+count_w);
                    *((*(md_unit_p+count_l)+count_u)->weights_temp+count_w)=*((*((layer_p+count_l)->weights+count_u))+count_w);
                }
            }
        }
    }

    /* allocate rank matrix for ranking nodes according to abs value of linear output */
    rank_mat=malloc(ntwk_info.layers*sizeof(long));
    for(count_l=0;count_l<ntwk_info.layers;count_l++)
    {
        *(rank_mat+count_l)=(int *)malloc((layer_p+count_l)->units*sizeof(int));
    }
}


static void md_file_open(void)  /*  opens training data file */
{
    printf("\nFile for training data: ");  /* get data file */
    gets(buf);  
    while((fp=fopen(buf,"r+"))==NULL)
    {
        fprintf(stderr,"\nError opening data file");
        printf("\nFile for data - type EXIT to exit program ");  /* get data file */
        gets(buf);
        if(buf=="EXIT")
            exit(1);
    }
    printf("\nFile to write results to : ");  /* get data file */
    gets(buf);  
    while((fp1=fopen(buf,"w+"))==NULL)
    {
        fprintf(stderr,"\nError opening data file");
        printf("\nFile for data - type EXIT to exit program ");  /* get data file */
        gets(buf);
        if(buf=="EXIT")
            exit(1);
    }
}
