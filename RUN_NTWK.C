#include "run_ntwk.h"
#include "neural.h"
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include<ctype.h> 
#include<io.h>


/* these static vars are used by the rn_get_data function adapted from md_get_data and bp_get_data functions */
static FILE *fp, *fp1,*fp2;  /* fp is data file, fp1 is file results are written to, and fp2 is the file where weights are located        */
static char buf[80];
static long int save_position;

static double *data_p;  /* input vectors */


static int count_l,count_u,count_w;  /*  counters for stepping through training vectors, layers, units, weights */

static int output_dim;

  
int eof_flag=0;  /*  set in get_data by a call to eof and returned to run_ntwk when data is exhausted */

/* rn_propagate() uses the layer sructure to propagate data vectores to the output layer */

/* write_output() takes the values of the nodes on the output layer and writes to the specified file */

/* rn_ntwk_init() inputs weights data from specified file and writes into the layers structure */

/* get_weight() returns the next weight in the weight file fp2 */

void run_network(struct layer *layer_p)
{
    output_dim=(layer_p+(ntwk_info.layers-1))->units;

    rn_file_open();
    
    rn_ntwk_init(layer_p);

    while(eof_flag!=1)
    {
        eof_flag=rn_get_data();
        rn_propagate(layer_p);
        write_output(layer_p);
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
}


static void rn_propagate(struct layer *layer_p)
{
    double temp=0,sum=0;
    for(count_l=0;count_l<ntwk_info.layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                sum=layer_p->bias * *(*(layer_p->weights+count_u));
                for(count_w=1;count_w<=ntwk_info.input_dim;count_w++)
                {
                    sum+= *(data_p+count_w-1) * *(*(layer_p->weights+count_u)+count_w);  
                }
                /* place the output */
                switch(layer_p->node_type)  /* add the rest of the activation funcitons  */
                {
                    case THRESHOLD:
                    {
                        if(sum>=0)
                        temp=1;
                        if(sum<0)
                        temp=-1;
                        *(layer_p->output+count_u)=temp;
                         break;
                    }
                    case LINEAR:
                    {
                        *(layer_p->output+count_u)=sum;
                        break;
                    }
                                        case LOGISTIC:
                    {
                        *(layer_p->output+count_u)=1/(1+exp(-sum));
                        break;
                    }
                    case TANH:
                    {
                        *(layer_p->output+count_u)=tanh(sum);
                        break;
                    }
                    default:
                    {
                        fprintf(stderr,"\nERROR: did not recgonize node_type presented to run_ntwk routine.");
                        fprintf(stderr,"\nPress any key to exit.");
                        gets(buf);
                        exit(1);
                    }
                }/* end switch */

            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                sum=(layer_p+count_l)->bias * *(*((layer_p+count_l)->weights+count_u));
                for(count_w=1;count_w<=(layer_p+count_l-1)->units;count_w++)
                {
                    sum+= *((layer_p+count_l-1)->output+count_w-1) * *(*((layer_p+count_l)->weights+count_u)+count_w);
                    
                }
                /* place the output */
                switch(layer_p->node_type)  /* this can be moved to the end of the  */
                {
                    case THRESHOLD:
                    {
                        if(sum>=0)
                        temp=1;
                        if(sum<0)
                        temp=-1;
                        *((layer_p+count_l)->output+count_u)=temp;
                        break;
                    }
                    case LINEAR:
                    {
                        *((layer_p+count_l)->output+count_u)=sum;
                        break;
                    }
                    case LOGISTIC:
                    {
                        *((layer_p+count_l)->output+count_u)=1/(1+exp(-sum));
                        break;
                    }
                    case TANH:
                    {
                        *((layer_p+count_l)->output+count_u)=tanh(sum); 
                        break;
                    }
                    default:
                    {
                        fprintf(stderr,"\nERROR: did not recgonize node_type presented to run_ntwk routine.");
                        fprintf(stderr,"\nPress any key to exit.");
                        gets(buf);
                        exit(1);
                    }
                }/* end switch */

            }
        }
    }

    

}

static void write_output(struct layer *layer_p)  /* writes the data vector, and the output vector as one record */
{
        fprintf(fp1,"input=:");
    for(count_w=0;count_w<ntwk_info.input_dim;count_w++)
    {
        fprintf(fp1,"%lg,",*(data_p+count_w));
    }

    fprintf(fp1,"\toutput=:");
    for(count_u=0;count_u<output_dim;count_u++)
    {
        if(count_u!=output_dim-1)
        {
            fprintf(fp1,"%lg,",*((layer_p+ntwk_info.layers-1)->output+count_u));
        }
        if(count_u==output_dim-1)
        {
            fprintf(fp1,"%lg\n",*((layer_p+ntwk_info.layers-1)->output+count_u));
        }
    }
}


static double get_weight(void)
{
    double weight;
    while(!feof(fp2))  /* get training info */
    {
        buf[0]=fgetc(fp2);
        if(buf[0]==':')
        break;
        if(feof(fp2))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp2,"%lg",&weight);    

    return weight;
    
}




static int rn_get_data(void)
{
    int flag=0;
    
    for(count_w=0;count_w<ntwk_info.input_dim;count_w++)  /*  get data vactor */
    {
        fscanf(fp,"%lg",(data_p+count_w));
        while(!feof(fp) && count_w!=ntwk_info.input_dim-1) 
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
        if(feof(fp))  /* end run when EOF is encountered at the end of a record,  ERROR above for EOF before complete record is entered */
        {
            flag=1;
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
   return flag;
}


 
static void rn_ntwk_init(struct layer *layer_p)
{

    if((data_p=malloc(ntwk_info.input_dim*sizeof(double)))==NULL)  /* allocate space for data vector */
        fprintf(stderr,"\nerror allocating data_p");
                       

    /*  initialize the weights for matrix of struct unit_train */
    for(count_l=0;count_l<ntwk_info.layers;count_l++) 
    {
        if(count_l==0)
        {
            for(count_u=0;count_u<layer_p->units;count_u++)
            {
                for(count_w=0;count_w<=ntwk_info.input_dim;count_w++)
                {
                    *(*(layer_p->weights+count_u)+count_w)=get_weight();  /*  experimented with removing what appear to be unnec. ()  */
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
            {
                for(count_w=0;count_w<=(layer_p+count_l-1)->units;count_w++)
                {
                    *(*((layer_p+count_l)->weights+count_u)+count_w)=get_weight();  /* experimented with removing () */
                }
            }
        }
    }
}


static void rn_file_open(void)  /*  opens files */
{
    printf("\nFile for data: "); 
    gets(buf);  
    while((fp=fopen(buf,"r+"))==NULL)
    {
        fprintf(stderr,"\nError opening data file");
        printf("\nFile for data - type EXIT to exit program ");  
        gets(buf);
        if(buf=="EXIT")
            exit(1);
    }
 
    printf("\nFile to write results to : ");  
    gets(buf);  
    while((fp1=fopen(buf,"w+"))==NULL)
    {
        fprintf(stderr,"\nError opening data file");
        printf("\nFile to write results to - type EXIT to exit program ");  
        gets(buf);
        if(buf=="EXIT")
            exit(1);
    }
    
    printf("\nFile where weights are to be found : ");  
    gets(buf);  
    while((fp2=fopen(buf,"r+"))==NULL)
    {
        fprintf(stderr,"\nError opening data file");
        printf("\nFile where weights can be found - type EXIT to exit program ");  
        gets(buf);
        if(buf=="EXIT")
            exit(1);
    }
}
    
