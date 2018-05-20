#include "neural.h"
#include "lms.h"
#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<io.h>

static char buf[80];
static FILE *fp,*fp1;
static long int save_position;
static double sum,*data_p,*desired_out,*error;
static int count,count_u,count_w,pass_num=1;
static int output_dim;

void lms(struct layer *layer_p,struct network ntwk_info)
{
    /* inserting safeguards to ensure that lms called with linear nodes and one layer */
    
    output_dim=layer_p->units;

    lms_file_open();
         
    if((data_p=malloc(ntwk_info.input_dim*sizeof(double)))==NULL) /* allocate space for input vector */
    {
        fprintf(stderr,"\nERROR: allocating space for input data array");
        fprintf(stderr,"\nPress any key to exit.");
        gets(buf);
        exit(1);
    }
    if((desired_out=malloc(output_dim*sizeof(double)))==NULL) /* allocate space for desired_out */
    {
        fprintf(stderr,"\nERROR: allocating space for desired output data array");
        fprintf(stderr,"\nPress any key to exit.");
        gets(buf);
        exit(1);
    }
    if((error=malloc(output_dim*sizeof(double)))==NULL) /* allocate space for error vector */
    {
        fprintf(stderr,"\nERROR: allocating space for desired output data array");
        fprintf(stderr,"\nPress any key to exit.");
        gets(buf);
        exit(1);
    }

    while(pass_num<ntwk_info.passes)
    {
        pass_num++;
        for(count=0;count<ntwk_info.num_vectors;count++)  
        {
            /* get data */
            lms_get_data();

            for(count_u=0;count_u<output_dim;count_u++)
            {
                
                sum=layer_p->bias * *(*(layer_p->weights+count_u));  /* initialize with bias term */
            
                for(count_w=1;count_w<=ntwk_info.input_dim;count_w++)
                    sum+=*(data_p+count_w-1) * *(*(layer_p->weights+count_u)+count_w);

                *(layer_p->output+count_u)=sum;    
                 
                *(error+count_u)=(*(desired_out+count_u)-sum);

                *(*(layer_p->weights+count_u))+=2 * ntwk_info.training_param * *(error+count_u) * layer_p->bias;
              
                for(count_w=1;count_w<=ntwk_info.input_dim;count_w++)
                    *(*(layer_p->weights+count_u)+count_w)+=2 * ntwk_info.training_param * *(error+count_u) * *(data_p+count_w-1);
            }/* end count_u */
            
            
        }/* end count */
        rewind(fp);
    }/* end pass_num while training loop */
    
    /* after the training passes are complete, we compute errors again and write results to file */
    lms_output_network(layer_p);

    rewind(fp);
    for(count=0;count<ntwk_info.num_vectors;count++)  
    {
        /* get data */
        lms_get_data();

        for(count_u=0;count_u<output_dim;count_u++)
        {
            sum=layer_p->bias * *(*(layer_p->weights+count_u));;  /* initialize with bias term */
            
            for(count_w=1;count_w<=ntwk_info.input_dim;count_w++)
               sum+=*(data_p+count_w-1) * *(*(layer_p->weights+count_u)+count_w);

            *(layer_p->output+count_u)=sum;    
                 
            *(error+count_u)=(*(desired_out+count_u)-sum);

        }/* end count_u */

        fprintf(fp1,"\ninput=:");
        for(count_u=0;count_u<ntwk_info.input_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(data_p+count_u));
        }
        fprintf(fp1,"  output=:");
        for(count_u=0;count_u<output_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(layer_p->output+count_u));
        }
        fprintf(fp1,"  desired out=:");
        for(count_u=0;count_u<output_dim;count_u++)
        {
            fprintf(fp1,"%lg ",*(desired_out+count_u));
        }

            
            
    }/* end count */
    fclose(fp);
    fclose(fp1);           
}


void lms_get_data(void)
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
    for(count_w=0;count_w<output_dim;count_w++)  /*  get output vector vector */
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
    /* we now neet to verify that we're at the end of a record,  or at least scan whitespace until data is encountered */
    buf[0]='\n';
    while(isspace(buf[0]))
    {
        save_position=ftell(fp);
        buf[0]=fgetc(fp);
        if( (count!=ntwk_info.num_vectors-1) && (feof(fp)) ) /* check that we have not reached EOF prematurely */
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





static void lms_file_open(void)  /* opens training data file */
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


static void lms_output_network(struct layer *layer_p)
{
    int count_u_p,count_w_p;

    fprintf(fp1,"\nThe activation values for the following were calculated using the"); 
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
    
    for(count_u_p=0;count_u_p<layer_p->units;count_u_p++)
    {
        fprintf(fp1,"\nweights   UINT[%d]\n",count_u_p);
        for(count_w_p=0;count_w_p<=ntwk_info.input_dim;count_w_p++)
        {
            fprintf(fp1,"=:%lg\n",*(*(layer_p->weights+count_u_p)+count_w_p));
        }
        fprintf(fp1,"linear output=\t%lg ",*(layer_p->output+count_u));
   }
}
