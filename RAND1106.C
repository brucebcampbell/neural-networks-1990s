#include "rand.h"
#include<stdio.h>
#include<malloc.h>
#include<time.h>

/* A random number generator adapted from Numerical Recipies In C - made minor changes and reomved bugs  */



/* Declarations for the random number generator */
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7   /* should be the smallest representable machine number, I set it to  */
#define RNMX (1.0-EPS)
                                         
time_t time_of_day;
struct tm *tm_buf;

long int *seed;


void rand_init(void)
{
    seed=malloc(sizeof(long));



        /* 123096: converted from Watcom _localtime to ANSI localtime fn. - verify that below still works */
    
    /* get some long int that varies in time - I will justify in a little snippet why this seed is random enough for our purposes */
    time_of_day = time( NULL );
    tm_buf=localtime(&time_of_day);
    *seed=time_of_day%clock()+(long)tm_buf->tm_sec;

    printf("\trandom seed=%ld",*seed);
     
    random(seed);
}
/* call rand2(void) from body of calling source code */
double rand2(void)
{
    return random(seed);
}
 
 /* Capable of generating a random sequence with a period >10^18        *
 * Essentially removes all correlations that may exist                 *
 * when calling rand() more than 2^15 times.  It is also               *
 * good for filling up k dim volumes with o(k) points,                 *
 * since rand tends to place points grouped in hyperplanes.            *
 * This algorithm is 2x speed of rand() and should not be              *
 * used for generating large numbers of random bits -                  *
 * there are easier ways...                                            *
 * Call function with *idum a negative integer to intitialize,         *
 * thereafter, do not alter idum between successive deviates           *
 * in a sequence                                                       */    
double random(long int *idum)
{
    int j;
    long int k;
    static long int idum2=123456789;
    static long int iy=0;
    static long int iv[NTAB];
    double temp;
    if(*idum<=0)    /* if called with the negative seed,  we load the shuffle table  */
    {
        if(-(*idum)<1)
        *idum=1;
        else
        *idum=-(*idum);
        idum2=(*idum);
        for(j=NTAB+7;j>=0;j--)
        {
            k=(*idum)/IQ1;
            *idum=IA1*(*idum-k*IQ1)-k*IR1;
            if(*idum<0)
            *idum+=IM1;
            if(j<NTAB)
            iv[j]=*idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ1;
    *idum=IA1*(*idum-k*IQ1)-k*IR1;
    if(*idum<0)
    *idum+=IM1;
    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2;
    if(idum2<0)
    idum2+=IM2;
    j=iy/NDIV;
    iy=iv[j]-idum2;
    iv[j]=*idum;
    if(iy<1)
    iy+=IMM1;
    if((temp=AM*iy)>RNMX)
    return RNMX;
    else
    return temp;
}
