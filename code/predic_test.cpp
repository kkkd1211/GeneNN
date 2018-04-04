#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;

void SynBox::clearPredicError()
{
    int i,j;
    for(i=0;i<Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            predic_error[i][j]=0;
        }
    }
}
double SynBox::testPredicError(const char type[2])
{
	clearPredicError();
    double total_predic_error=0;
    double x[7],xl[4],xr[4];
    int i,j,row;
    readData(type);
    for(i=0;i<Nt-1;i++)
    {
        row=i;
        for(j=0;j<Nx;j++)
        {
            x[0]=kniData[row][j];
            x[1]=hbData[row][j];
            x[2]=krData[row][j];
            x[3]=gtData[row][j];
            x[4]=bcdData[j];
            x[5]=nosData[j];
            x[6]=tllData[j];
            if(j!=0)
            {
                xl[0]=kniData[row][j-1];
                xl[1]=hbData[row][j-1];
                xl[2]=krData[row][j-1];
                xl[3]=gtData[row][j-1];
            }
            else if(j==0)
            {
                xl[0]=-1;
                xl[1]=-1;
                xl[2]=-1;
                xl[3]=-1;
            }
            if(j!=Nx-1)
            {
                xr[0]=kniData[row][j+1];
                xr[1]=hbData[row][j+1];
                xr[2]=krData[row][j+1];
                xr[3]=gtData[row][j+1];
            }
            else if(j==Nx-1)
            {
                xr[0]=-1;
                xr[1]=-1;
                xr[2]=-1;
                xr[3]=-1;
            }
            tg[0]=kniData[row+1][j]-kniData[row][j];
            tg[1]=hbData[row+1][j]-hbData[row][j];
            tg[2]=krData[row+1][j]-krData[row][j];
            tg[3]=gtData[row+1][j]-gtData[row][j];
            predic(x,xl,xr);
            de();
            err();
            predic_error[row][j]=loss_error;
            total_predic_error+=predic_error[row][j]*predic_error[row][j];
        }///loop(Nx) 0 to 100 in one row
    }//loop step(Nt)
    total_predic_error=sqrt(total_predic_error);
    return(total_predic_error);
}//testPredicError

void SynBox::savePredicError(const char type[2],int dataNO,double predicErr)
{
    FILE *f_predicErr;
    char predicErrFile[50];
    int i,j;
    sprintf(predicErrFile,"predicError/%s%d(%.2f).txt",type,dataNO,predicErr);
    f_predicErr=fopen(predicErrFile,"w");
    for(i=0;i<Nt-1;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fprintf(f_predicErr,"%f\t",predic_error[i][j]);
        }
        fprintf(f_predicErr,"\n");
    }
}
