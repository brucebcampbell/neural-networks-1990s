#include<stdio.h>
#include<stdlib.h>
#include "neural.h"
#include "bk_prop.h"
#include "rand.h"
#include <math.h>
#include<ctype.h> 
#include<io.h> 

/* data is loaded into matrix of bp_unit_train structures for training, trained network is saved back to layer structure */               
struct bp_unit_train{ 
    double *weights;
    double linear_output; 
    double activation;
    double derivative;
    double delta;
    int rank;};    
struct bp_unit_train **bp_unit_p;

static struct bp_unit_train *x;  /* optimization variable to be used where layer and unit are fixed */

/* the variable below were borowed from the min_disturbance module */

static FILE *fp, *fp1;  /*fp is data input file, fp1 is the output file */     
static char buf[80];
static long int save_position;

static double *data_p,*desired_out;  /* input & output vectors */

/* counter for the number of training passes for the training data */
static int pass_num=0;  

/* indicates the random choice of training vector selected from file, used in min_disturbance() and bp_get_data() */
static int vector_number;

/* counters for stepping through training vectors, layers, units, weights */
static int count,count_l,count_u,count_w;

static double error;  

/* an array of ptrs to error sums, indexed over training passes */
static double **global_error; 

static int output_dim;

  
void back_prop(struct layer *layer_p,struct network ntwk_info)
{
    output_dim=(layer_p+(ntwk_info.layers-1))->units;

    if(ntwk_info.layers!=2 && ntwk_info.layers!=3)
    {
        fprintf(stderr,"\nERROR: backpropagation training algorithm should be run with either one or two hidden layers");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
        exit(1);
    }

    bp_file_open();
    
    bp_unit_p_init(layer_p);

    /* training routine,  runs through training vectors number of times specified in network info file */
    for(pass_num=0;pass_num<ntwk_info.passes;pass_num++)
    {        
        for(vector_number=0;vector_number<ntwk_info.num_vectors;vector_number++)
        {
                        /* puts data from fp into data_p and desired_out  */
            bp_get_data(); 
            
                        /* bp_propagate the input,  the output is present at both bp_unit_p and layer_p */
            bp_propagate(layer_p,data_p);  
  
            error=bp_error_comp(layer_p,desired_out);  /*  compute the bipolar error */

            *(*(global_error+pass_num)+vector_number)=error;

            bp_update_weight(layer_p);
        }      
        rewind(fp);



    } 
    /* send network to output file  */
    bp_output_network(layer_p);

    /*send error results to file */
    fprintf(fp1,"\nError results for each training vector.");
    for(vector_number=0;vector_number<ntwk_info.num_vectors;vector_number++)  
    {
        bp_get_data();
        bp_propagate(layer_p,data_p);
        fprintf(fp1,"\ne[%d]=:%lg  input=:",vector_number,bp_error_comp(layer_p,desired_out));
        for(count_u=0;count_u<ntwk_info.input_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(data_p+count_u));
        }
        fprintf(fp1,"  output=:");
        for(count_u=0;count_u<output_dim;count_u++)
        {
            fprintf(fp1,"%lg ",(*(bp_unit_p+ntwk_info.layers-1)+count_u)->activation);
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
        fprintf(fp1,"\ng_e[%d]= ",count);
        for(count_u=0;count_u<ntwk_info.num_vectors;count_u++)
        {
            fprintf(fp1," :%lg",*(*(global_error+count)+count_u));
        }
    }
    printf("\nAll Done.");             
    fclose(fp1);
    fclose(fp);
}



/*bp_propagates input vector throught the network, stores output in the layer_p structure and the 
  bp_unit_p structure.  The activation is stored in layer_p while only the linear output of a node 
  is stored in the unit_p structure   */
static void bp_propagate(struct layer *layer_p,double *data_p)
{
    double sum;
    for(count_l=0;count_l<ntwk_info.layers;count_l++)
    {
        if(count_l==0)
        {
            /* go trought the frist layer, applying input vector to each node and computing outputs */
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                x=*(bp_unit_p+count_l)+count_u;
                
                sum=(layer_p)->bias * *(x->weights);  /* compute bias term */

                /* We have to remember that we're using count_w to step through the units 
                   on the pervious layer which start at 0, but we're multiplying by non-bias weights 
                   which start at 1, so we subtract off 1 from the count in the first term of the sum */ 

                for(count_w=1;count_w<=ntwk_info.input_dim;count_w++)
                    sum+=*(data_p+count_w-1) * *(x->weights+count_w);
              
                x->linear_output=sum;
                 
                switch(layer_p->node_type)   
                {
                    case THRESHOLD:
                    {
                        fprintf(stderr,"\nERROR: backpropagation was called with non_differentiable activation fn");
                                                fprintf(stderr,"\nPress any key to exit.");
                                                gets(buf);
                                                exit(1);
                        break;
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
                        fprintf(stderr,"\nERROR: did not recgonize node_type presented to min_disturbance training.");
                        fprintf(stderr,"\nPress any key to exit.");
                                                gets(buf);
                                                exit(1);
                    }
                }/* end switch */
            }/* end unit count for layer 0 */
        }/* end if for layer 0 */
            
        if(count_l!=0)
        {
            
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                x=*(bp_unit_p+count_l)+count_u;
                
                sum=(layer_p+count_l)->bias * *(x->weights);  /* bias term */
                 
                for(count_w=1;count_w<=(layer_p+count_l-1)->units;count_w++)
                    sum+= (*(bp_unit_p+count_l-1)+count_w-1)->activation * *(x->weights+count_w);
                    
                x->linear_output=sum;
                    
                switch((layer_p+count_l)->node_type)
                {
                    case THRESHOLD:
                    {
                        fprintf(stderr,"\nERROR: backpropagation was called with non_differentiable activation fn");
                                                fprintf(stderr,"\nPress any key to exit.");
                                                gets(buf);
                                                exit(1);
                        break;
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
                        fprintf(stderr,"\nERROR: did not recognise node_type presented to min_disturbance training.");
                        fprintf(stderr,"\nPress any key to exit.");
                                                gets(buf);
                                                exit(1);
                    }
               }/* end switch */
            }/* end unit count for layer count_l */
        }/* end if for layer !=0 */
    }/* end layers count */
}    

/* computes errors - stores in the unit_p structure */
static double bp_error_comp(struct layer *layer_p,double *desired_out)  
{
    int count_out,count_hidden1;
    double temp,err;
    if(ntwk_info.layers>3)
    {
        fprintf(stderr,"\nERROR: attempted backprop triaining on a network with more than two hidden layers");
        fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
    }
    for(count_l=ntwk_info.layers-1;count_l>=0;count_l--)
    {

  /* We have proper handling for a 2 hidden layer network, but the code
     as written does not handle 1 hidden layer.  We direct user to use
     lms training if one layer gradient descent is desired  */

        if(ntwk_info.layers==3)
        {
            switch(count_l)
            {
                case 2:
                {
                    temp=0;
                    for(count_u=0;count_u<(layer_p+2)->units;count_u++)
                    {
                        temp+=pow((*(bp_unit_p+2)+count_u)->activation - *(desired_out+count_u),2);
                        (*(bp_unit_p+2)+count_u)->delta=(*(desired_out+count_u) - (*(bp_unit_p+2)+count_u)->activation) * (*(bp_unit_p+2)+count_u)->derivative;
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
                    for(count_u=0;count_u<(layer_p+1)->units;count_u++)
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
                    fprintf(stderr,"\nERROR: layer count out of range in backprop trianing algorithm");
                    fprintf(stderr,"\nPress any key to exit.");
                                        gets(buf);
                                        exit(1);
                }
            }/* end switch */
        }/* end if layer==3 */

        if(ntwk_info.layers==2)
        {
            switch(count_l)
            {
                case 1:
                {
                    temp=0;
                    for(count_u=0;count_u<(layer_p+1)->units;count_u++)
                    {
                        temp+=pow((*(bp_unit_p+1)+count_u)->activation - *(desired_out+count_u),2);
                        (*(bp_unit_p+1)+count_u)->delta=(*(desired_out+count_u) - (*(bp_unit_p+1)+count_u)->activation) * (*(bp_unit_p+1)+count_u)->derivative;
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
                    fprintf(stderr,"\nERROR: layer count out of range in backprop trianing algorithm");
                    fprintf(stderr,"\nPress any key to exit.");
                                        gets(buf);
                                        exit(1);
                }
            }/* end switch */
        }/* end if layer==2 */            
    }/*end count_l */
    return err;
}

static void bp_update_weight(struct layer *layer_p)
{
    for(count_l=0;count_l<ntwk_info.layers;count_l++)
    {
        if(count_l==0)
        {
            /* go trough the first layer, applying input vector to each node and computing outputs */
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                x=*(bp_unit_p)+count_u;
                
                *(x->weights)+=ntwk_info.training_param * x->delta * layer_p->bias;
                
                for(count_w=1;count_w<=ntwk_info.input_dim;count_w++)
                {
                    *(x->weights+count_w)+=ntwk_info.training_param * x->delta * *(data_p+count_w-1);
                }
            }/* end unit count for layer 0 */
        }/* end if for layer 0 */
            
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                x=*(bp_unit_p+count_l)+count_u;

                *(x->weights)+=ntwk_info.training_param * x->delta * (layer_p+count_l)->bias;
                
                for(count_w=1;count_w<=(layer_p+count_l-1)->units;count_w++)
                {
                    *(x->weights+count_w)+=ntwk_info.training_param * x->delta * (*(bp_unit_p+count_l-1)+count_w-1)->activation;
                }    
            }/* end unit count for layer count_l */
        }/* end if for layer !=0 */
    }/* end layers count  */
    
        
    
  
    
}

/* This prints training results to file specified by FILE *fp1 */
static void bp_output_network(struct layer *layer_p)
{
    int count_l_p,count_u_p,count_w_p;

    fprintf(fp1,"\nThe following activations, deltas, and linear outputs were calculated using the");
    fprintf(fp1,"\ntraining vector=");
    for(count_w_p=0;count_w_p<ntwk_info.input_dim;count_w_p++) /* write the current training vector */
    {
        fprintf(fp1,"%lg, ",*(data_p+count_w_p));
    }
    fprintf(fp1,"\ndesired out=");
    for(count_w_p=0;count_w_p<output_dim;count_w_p++) /* write the desired_output vector */
    {
        fprintf(fp1,"%lg, ",*(desired_out+count_w_p));
    }
    
    for(count_l_p=0;count_l_p<ntwk_info.layers;count_l_p++)  /* write the bp_unit_train structure */
    {
        fprintf(fp1,"\n\t\t\tLAYER [%d]",count_l_p);
        if(count_l_p==0)
        {
            for(count_u_p=0;count_u_p<layer_p->units;count_u_p++)
            {
                fprintf(fp1,"\nweights   UINT[%d][%d]\n",count_l_p,count_u_p);
                for(count_w_p=0;count_w_p<=ntwk_info.input_dim;count_w_p++)
                {
                    fprintf(fp1,"=:%lg\n",*((*(bp_unit_p+count_l_p)+count_u_p)->weights+count_w_p));
                }
                fprintf(fp1,"linear output=\t%lg ",(*(bp_unit_p+count_l_p)+count_u_p)->linear_output);
                fprintf(fp1,"  activation=\t%lg",(*(bp_unit_p+count_l_p)+count_u_p)->activation);
                fprintf(fp1,"  delta=\t%lg",(*(bp_unit_p+count_l_p)+count_u_p)->delta);
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
                fprintf(fp1,"linear output=\t%lg ",(*(bp_unit_p+count_l_p)+count_u_p)->linear_output);
                fprintf(fp1,"  activation=\t%lg",(*(bp_unit_p+count_l_p)+count_u_p)->activation);
                fprintf(fp1,"  delta=\t%lg",(*(bp_unit_p+count_l_p)+count_u_p)->delta);
            }
        }
    }
        
    fprintf(fp1,"\ndesired out=");
    for(count_w_p=0;count_w_p<output_dim;count_w_p++) /* write the desired_output vector */
    {
        fprintf(fp1,"%lg, ",*(desired_out+count_w_p));
    }
}


void bp_get_data(void)
{
    for(count_w=0;count_w<ntwk_info.input_dim;count_w++)  /* get training vector */
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
    for(count_w=0;count_w<output_dim;count_w++)  /* get output vector vector */
    {
        fscanf(fp,"%lg",(desired_out+count_w));
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
    /* We now neet to verify that we're at the end of a record,  or at least scan whitespace until data is encountered */
    buf[0]='\n';
    while(isspace(buf[0]))
    {
        save_position=ftell(fp);
        buf[0]=fgetc(fp);
        if( (vector_number!=ntwk_info.num_vectors-1) && (feof(fp)) ) /* check that we have not reached EOF prematurely */
        {
            fprintf(stderr,"\nERROR: unexpected EOF in data file");
            fprintf(stderr,"\nPossible cause is that # training vectors in parameter file doesn't equal # vectors in data file");
            fprintf(stderr,"\nThe current number of data vectors is %d, enter a new number or -1 to exit",ntwk_info.num_vectors);
            scanf("%d",&ntwk_info.num_vectors);
            if(ntwk_info.num_vectors) 
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

void bp_unit_p_init(struct layer *layer_p)
{
    if((global_error=malloc(ntwk_info.passes*sizeof(long)))==NULL)
    {
        fprintf(stderr,"\nERROR: allocating aray of ptrs for global error");
        fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
    }        
    for(count=0;count<ntwk_info.passes;count++)
    {
        if((*(global_error+count)=(double *)malloc(ntwk_info.num_vectors*sizeof(double)))==NULL)
        {
            fprintf(stderr,"\nERROR: allocating array of doubles for error");
            fprintf(stderr,"\nPress any key to exit.");
                        gets(buf);
                        exit(1);
        }
    }
    if((data_p=malloc(ntwk_info.input_dim*sizeof(double)))==NULL)  /* allocate space for data vector */
        fprintf(stderr,"\nerror allocating data_p");
    if((desired_out=malloc(output_dim*sizeof(double)))==NULL)  /* allocate space for desired output vector */
        fprintf(stderr,"\nerror allocating desired_out");
                       
    if((bp_unit_p=malloc(ntwk_info.layers*sizeof(long)))==NULL)  /* allocate array of pointers to array of pointers to units */
        fprintf(stderr,"\nerror allocating bp_unit_p"); 
    for(count_l=0;count_l<ntwk_info.layers;count_l++)  /* allocate vectors of struct bp_unit_train corresponding to layers */
    {
        if((*(bp_unit_p+count_l)=(struct bp_unit_train *)malloc((layer_p+count_l)->units*sizeof(struct bp_unit_train)))==NULL)
            fprintf(stderr,"\nerror allocating row bp_unit_train");
    }

    for(count_l=0;count_l<ntwk_info.layers;count_l++)  /* alocate space for weights remembering that bias weight is always the first one */
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++) /* allocate array of weight vectors for each bp_unit_train in input layer */
            {
                if(((*(bp_unit_p+count_l)+count_u)->weights=(double *)malloc((ntwk_info.input_dim+1)*sizeof(double)))==NULL)
                    fprintf(stderr,"\nerror allocating weights ");
            }
        }
        if(count_l!=0)  /* allocate array of weight vectors for each bp_unit_train in the remaining layers */
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                if(((*(bp_unit_p+count_l)+count_u)->weights=(double *)malloc(((layer_p+count_l-1)->units+1)*sizeof(double)))==NULL)
                    fprintf(stderr,"\nerror allocating weights layer !=0");
            }
        }
    }
    /* initialize the weights for matrix of struct bp_unit_train */
    for(count_l=0;count_l<ntwk_info.layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                for(count_w=0;count_w<ntwk_info.input_dim+1;count_w++)
                {
                    *((*(bp_unit_p+count_l)+count_u)->weights+count_w)=*((*(layer_p->weights+count_u))+count_w);
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                for(count_w=0;count_w<(layer_p+count_l-1)->units+1;count_w++)
                {
                    *((*(bp_unit_p+count_l)+count_u)->weights+count_w)=*((*((layer_p+count_l)->weights+count_u))+count_w);
                }
            }
        }
    }
}


static void bp_file_open(void)  /* opens training data file */
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
    
