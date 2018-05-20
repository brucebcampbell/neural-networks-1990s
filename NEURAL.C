
/*******************************************************************
   This code module dynamically allocates a configurable
   multilayer topology for a feed forward neural network
   data on the network topology is read from a data file.
   training and running the network is done from,
   Modules lms,min_dist,bak_prop are the kernal of what will be attached
   to a windows front end some day.The random number generator rand2()
   was adapted from Numerical Recipies in C, Press et.al.   
*********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "neural.h"
#include "rand.h"
#include "lms.h"
#include "min_dist.h"
#include "bk_prop.h"
#include "run_ntwk.h"

char buf[80];

#define LAYERS 5 /* The maximum allowable # of layers     */


/* These two structs contain all of the important info that is
   to be used in the training modules.  They are declaired in the
   header file neural.h        */
struct network ntwk_info;
struct layer layers[LAYERS];


double upper_range,lower_range;  /* upper and lower bounds on random initialization of weights for units */

int top_gen(void);  /* Topology generator */

int info_scan(FILE *fp);

void clear_kb(void);
                                                              
main_NN()
{
    
    FILE *fp;
    char buf[256];
    int run_ans=-1;
    
    printf("\nEnter data file of type *.top for network topology:");
    gets(buf);
    
        while((fp=fopen(buf,"r+"))==NULL)
    {
        fprintf(stderr,"\nError opening data file");
        printf("\nFile for data - type EXIT to exit program ");  /* get data file */
        gets(buf);
        if(buf=="EXIT")
            exit(1);
    }

    info_scan(fp);

    top_gen();
    
    while(run_ans!=0 && run_ans!=1)
    {
        printf("\nDo you want to run this network or train it? 1=RUN 0=TRAIN:");
        scanf("%d",&run_ans);
    }
    clear_kb();
    
    if(run_ans==1)
    run_network(layers);
    
    if(run_ans==0)
    {
        switch(ntwk_info.training)  
        {                           
            case LMS:
                {
                    lms(layers,ntwk_info);
                    break;
                }

            case MIN_DISTURBANCE: 
                {
                    min_disturbance(layers,ntwk_info);
                    break;
                }
            case BACKPROPAGATION: 
                {
                    back_prop(layers,ntwk_info);
                    break;
                }    
            default: 
                {
                    fprintf(stderr,"\nError in safeguards for training options");
                    fprintf(stderr,"\nPress any key to exit.");
                    gets(buf);
                    exit(1);
                }
        }/* end switch */
    }/* endif */
        return 0;
} /*  end main       */


/* scans info from network info file int ntwk_info data structure */
int info_scan(FILE *fp)
{
    char buf[256];
    int count_l;
    /* get number of layers from file */
    while(!feof(fp))
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%d",&ntwk_info.layers);  
        
    while(!feof(fp))  /* get training info */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%d",&ntwk_info.training);    
    
    while(!feof(fp))  /* get input_dimension */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%d",&ntwk_info.input_dim);    
    
    for(count_l=0;count_l<5;count_l++) /* get units for each layer, 0 had better be scanned from file for layer#'s > ntwk_info.layers */
    {
        while(!feof(fp))
        {
            buf[0]=fgetc(fp);
            if(buf[0]==':')
            break;
            if(feof(fp))
            {
                fprintf(stderr,"\nERROR: unexpected end of file.");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }
        if(count_l<ntwk_info.layers)  /* check that we're about to scan data for a layer that exists */
        fscanf(fp,"%d",&layers[count_l].units);
        else  /* if not we scan extra data into buf */
        fscanf(fp,"%s",buf);    
    }
     
    for(count_l=0;count_l<5;count_l++) /* get bias for each layer, 0 is entered from file for no bias,   */
    {
        while(!feof(fp))
        {
            buf[0]=fgetc(fp);
            if(buf[0]==':')
            break;
            if(feof(fp))
            {
                fprintf(stderr,"\nERROR: unexpected end of file.");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }
        if(count_l<ntwk_info.layers)
        fscanf(fp,"%lg",&layers[count_l].bias);
        else
        fscanf(fp,"%s",buf);
    }
        
    for(count_l=0;count_l<5;count_l++) /* get node_type for each layer    */
    {
        while(!feof(fp))
        {
            buf[0]=fgetc(fp);
            if(buf[0]==':')
            break;
            if(feof(fp))
            {
                fprintf(stderr,"\nERROR: unexpected end of file.");
                fprintf(stderr,"\nPress any key to exit.");
                gets(buf);
                exit(1);
            }
        }
        if(count_l<ntwk_info.layers)
        fscanf(fp,"%d",&layers[count_l].node_type);
        else
        fscanf(fp,"%s",buf);
    }

    while(!feof(fp)) /* get upper range of initial weights */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%lg",&upper_range);
    
    while(!feof(fp))  /* get lower range of initial weights */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%lg",&lower_range);

    while(!feof(fp))  /* get number of input vectors - training or running mode */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%d",&ntwk_info.num_vectors);
    
    while(!feof(fp))  /* get training parameter */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%lg",&ntwk_info.training_param);

    while(!feof(fp))  /* get number of training passes */
    {
        buf[0]=fgetc(fp);
        if(buf[0]==':')
        break;
        if(feof(fp))
        {
            fprintf(stderr,"\nERROR: unexpected end of file.");
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
    }
    fscanf(fp,"%d",&ntwk_info.passes);
       
    fclose(fp);
    
    return 0;  
}  /* end info scan */

            
int top_gen(void)
{
    int count_l,count_u,count_w;  /* counters for stepping through layers,units,weights, and input_vectors */
        
    rand_init();  /*  initialize random number generator  - there may be a problem with rand module preventing 16bit win app from being run under win95 */
    
    if(ntwk_info.training==LMS)
    {
        /*insert code to verify that were dealing with adaline, need LINEAR node type and single unit in network */
    }
    
    /* verify training type recognized */


    /*  Allocate network structure in memory */
    for(count_l=0;count_l<ntwk_info.layers;count_l++) 
    {
        /* dynamically allocate space for output vector for layer  */
        if((layers[count_l].output=malloc(layers[count_l].units*sizeof(double)))==NULL)
        {
            fprintf(stderr,"\nError allocating output array for layer %d",count_l);
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }
        
        /*  Allocate space for array of ptrs to weight vectors based on number of units on layer  */
        if((layers[count_l].weights=malloc(layers[count_l].units*sizeof(long)))==NULL)
        {
            fprintf(stderr,"\nError allocating array of ptrs to weight vectors for layer %d",count_l);
            fprintf(stderr,"\nPress any key to exit.");
            gets(buf);
            exit(1);
        }        
    }

    /*  Initialize weights using random number generator rand2     */
    for(count_l=0;count_l<ntwk_info.layers;count_l++)
    {
        if(count_l==0)
        {
            /* don't need above line? bias for each layer is in layers[count_l].bias already */
            for(count_u=0;count_u<layers[0].units;count_u++)
            {
                if((*(layers[0].weights+count_u)=(double*)malloc((ntwk_info.input_dim+1)*sizeof(double)))==NULL)
                {
                    fprintf(stderr,"\nError allocationg space for value of weight %d on layer 0.",count_w);
                    fprintf(stderr,"\nPress any key to exit.");
                    gets(buf);
                    exit(1);
                }       
                                    
                for(count_w=0;count_w<=ntwk_info.input_dim;count_w++)
                {
                    *((*(layers[0].weights+count_u))+count_w)=lower_range+rand2()*(upper_range-lower_range);
                }
            }
        }
        if(count_l!=0)
        {
            for(count_u=0;count_u<(layers[count_l].units);count_u++)
            {
                if((*(layers[count_l].weights+count_u)=(double*)malloc((layers[count_l-1].units+1)*sizeof(double)))==NULL)
                {
                    fprintf(stderr,"\nError allocating space for value of weight %d on layer %d.",count_w,count_l);
                    fprintf(stderr,"\nPress any key to exit.");
                    gets(buf);
                    exit(1);
                }
                
                for(count_w=0;count_w<=layers[count_l-1].units;count_w++)
                {
                    *((*(layers[count_l].weights+count_u))+count_w)=lower_range+rand2()*(upper_range-lower_range);
                }
            }
        }          
    }
    return 0;  
}  /* end top_gen */


void clear_kb(void)
{
    char junk[80];
    gets(junk);
}

    /*  The folllowing code prints network info to screen from the layers structure
        It is no longer in use, but archive here for the future

    printf("\n");
    printf("\nThe network has %d layers.  ",ntwk_info.layers);
    printf("\nThere are %d input nodes, %d nodes on layer 1 and %d nodes on the output layer",layer_p->units,(layer_p+1)->units,(layer_p+2)->units);
    printf("\nThe dimension of the input vector is %d. ",input_dim);
    printf("The dimension of the output vector is %d.",output_dim);
    for(count_l=0;count_l<ntwk_info.layers;count_l++)
    {
        switch ((layer_p+count_l)->bias)
        {
            case 0:{
            if(count_l==0)
            {
                printf("\nThere is a bias for this layer.");
                printf("\nThe weights for the first layer:");
                for(count_u=0;count_u<layer_p->units;count_u++)
                { 
                    printf("\n\n");
                    for(count_w=0;count_w<input_dim+1;count_w++)
                        printf("%lg ",*((*(layer_p->weights+count_u))+count_w));
                }
            }
            if(count_l!=0)
            {
                printf("\nThere is a bias for this layer.");
                printf("\nThe weights for layer %d:",count_l+1);
                for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
                {
                    printf("\n\n");
                    for(count_w=0;count_w<(layer_p+count_l-1)->units+1;count_w++)
                        printf("%lg",*((*((layer_p+count_l)->weights+count_u))+count_w));
                }
            }
            break;}
            default:{
            if(count_l==0)
            {
                printf("\nThe weights for the first layer:");
                for(count_u=0;count_u<layer_p->units;count_u++)
                {
                    printf("\n\n");

                    for(count_w=0;count_w<input_dim;count_w++)
                        printf("%lg ",*((*(layer_p->weights+count_u))+count_w));
                }
            }
            if(count_l!=0)
            {
                printf("\nThe weights for layer %d:",count_l+1);
                for(count_u=0;count_u<(layer_p+count_l)->units;count_u++)
                {
                    printf("\n\n");
                    for(count_w=0;count_w<(layer_p+count_l-1)->units;count_w++)
                        printf("%lg",*((*((layer_p+count_l)->weights+count_u))+count_w));
                }
            }
            }
        }
    } end  archived print statements */

            
    
