#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;

void SynBox::train(const char type[2],int step)
{
    printf("trainning %s...\n",type);
    h=5.0;
    ln_rate=0.5;
    d_rate=0.4/step;
    double x[7],xl[4],xr[4];
    int i,j,kk,row;
    readData(type);
    for(i=0;i<step;i++)
    {
        row=(int)((rand()/2147483647.0)*Nt);
        if(kniData[row][0]!=kniData[row][0])
            continue;
        if(row==Nt-1)
            continue;
        for(kk=0;kk<Nx;kk++) //kk<Nx
        {
            j=(int)((rand()/2147483647.0)*Nx);
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
            para_update();
        }///loop(j) 0 to 100 in one row
        ln_rate-=d_rate;
    }//loop step(i)
}//train
void SynBox::savePara(int dataNO)
{
    FILE *f_para;
    char paraFile[50];
    int i,j;
    sprintf(paraFile,"para/para%d.txt",dataNO);
    printf("saving to file:%s...",paraFile);
    f_para=fopen(paraFile,"w");
    fprintf(f_para,"k:\n");
    for(i=0;i<7;i++)
        fprintf(f_para,"%f\t",k[i]);
    fprintf(f_para,"\nv:\n");
    for(i=0;i<7;i++)
    {
        for(j=0;j<4;j++)
            fprintf(f_para,"%f\t",v[i][j]);
        fprintf(f_para,"\n");
    }
    fprintf(f_para,"beta:\n%f\nD:\n%f",beta,D);
    fclose(f_para);
    printf("done!\n\n");
}//savePara
void SynBox::set(const char file[])
{
    printf("\nloading data from \"%s\"...",file);
    FILE *fp;
    fp=fopen(file,"r");
    int i,j;
    fscanf(fp,"k:\n");
    for(i=0;i<7;i++)
    {
        fscanf(fp,"%lf\t",&k[i]);
    }
    fscanf(fp,"\nv:\n");
    for(i=0;i<7;i++)
    {
        for(j=0;j<4;j++)
        {
            fscanf(fp,"%lf\t",&v[i][j]);
        }
        fscanf(fp,"\n");
    }
    fscanf(fp,"beta:\n%lfD:\n%lf",&beta,&D);
    fclose(fp);
    printf("set finished!\n\npara:\n");
    for(i=0;i<7;i++)
        printf("%f\t",k[i]);
    printf("\n");
    for(i=0;i<7;i++)
    {
        for(j=0;j<4;j++)
        {
            printf("%f\t",v[i][j]);
        }
        printf("\n");
    }
    printf("%f\n\n",beta);
}//set
void SynBox::para_update()
{
    int i,j;
    for(i=0;i<7;i++)
    {
        k[i]-=n_ek[i]*h*ln_rate;
        if(k[i]<0)
            k[i]=k_epsilon;
        for(j=0;j<4;j++)
            v[i][j]-=n_ev[i][j]*h*ln_rate;
    }
    beta-=n_eb*h*ln_rate;
    if(beta<0)
        beta=k_epsilon;
    D-=n_eD*h*ln_rate;
    if(D<0)
        D=k_epsilon;
//    ln_rate-=d_rate;
}//para_update
