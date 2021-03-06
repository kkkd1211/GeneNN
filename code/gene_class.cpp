#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;


SynBox::SynBox()
{
    printf("Hello new box!!\n\n");
	dt=0.0001;
	ln_rate=0.5;
	h=5.0;
	int i,j;
    beta = 0.02 * (rand()/2147483647.0) ;
    D=pow(10,rand()/2147483647.0*2+2);  //1000
    for(i=0;i<7;i++)
    {
        k[i]=2.0*(rand()/2147483647.0);
        for(j=0;j<4;j++)
        {
            v[i][j]=20.0*(rand()/2147483647.0)-10.0;
        }
    }
    for(i=0;i<4;i++)
    {
        for(j=0;j<Nx;j++)
            curveErrorWeight[i][j]=1.0;
    }
}
