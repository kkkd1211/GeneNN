#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;
/*
void SynBox::train(const char type[2],int step)
{
    printf("trainning %s...\n",type);
    d_rate=0.4/step;
    char filekni[50];
    char filehb[50];
    char filekr[50];
    char filegt[50];
    char filebcd[50];
    char filenos[50];
    char filetll[50];
    sprintf(filekni,"data/%s/kni.txt",type);
    sprintf(filehb,"data/%s/hb.txt",type);
    sprintf(filekr,"data/%s/kr.txt",type);
    sprintf(filegt,"data/%s/gt.txt",type);
    sprintf(filebcd,"data/%s/bcd.txt",type);
    sprintf(filenos,"data/%s/nos.txt",type);
    sprintf(filetll,"data/%s/tll.txt",type);
    FILE *fkni,*fhb,*fkr,*fgt,*fbcd,*fnos,*ftll;
    double kni[Nt][Nx],hb[Nt][Nx],kr[Nt][Nx],gt[Nt][Nx];
    double bcd[Nx],nos[Nx],tll[Nx];
    double x[7],xl[4],xr[4];
    int i,j,kk,row;
    fkni=fopen(filekni,"r");
    fhb=fopen(filehb,"r");
    fkr=fopen(filekr,"r");
    fgt=fopen(filegt,"r");
    for(i=0;i<Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fscanf(fkni,"%lf ",&kni[i][j]);
            fscanf(fhb,"%lf ",&hb[i][j]);
            fscanf(fkr,"%lf ",&kr[i][j]);
            fscanf(fgt,"%lf ",&gt[i][j]);
        }
    }
    fclose(fkni);
    fclose(fhb);
    fclose(fkr);
    fclose(fgt);
    fbcd=fopen(filebcd,"r");
    fnos=fopen(filenos,"r");
    ftll=fopen(filetll,"r");
    for(i=0;i<Nx;i++)
    {
        fscanf(fbcd,"%lf ",&bcd[i]);
        fscanf(fnos,"%lf ",&nos[i]);
        fscanf(ftll,"%lf ",&tll[i]);
    }
    fclose(fbcd);
    fclose(fnos);
    fclose(ftll);

    for(i=0;i<step;i++)
    {
        row=(int)((rand()/2147483647.0)*Nt);
        if(kni[row][0]!=kni[row][0])
            continue;
        if(row==Nt-1)
            continue;
        for(kk=0;kk<Nx;kk++) //kk<Nx
        {
            j=(int)((rand()/2147483647.0)*Nx);
            x[0]=kni[row][j];
            x[1]=hb[row][j];
            x[2]=kr[row][j];
            x[3]=gt[row][j];
            x[4]=bcd[j];
            x[5]=nos[j];
            x[6]=tll[j];
            if(j!=0)
            {
                xl[0]=kni[row][j-1];
                xl[1]=hb[row][j-1];
                xl[2]=kr[row][j-1];
                xl[3]=gt[row][j-1];
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
                xr[0]=kni[row][j+1];
                xr[1]=hb[row][j+1];
                xr[2]=kr[row][j+1];
                xr[3]=gt[row][j+1];
            }
            else if(j==Nx-1)
            {
                xr[0]=-1;
                xr[1]=-1;
                xr[2]=-1;
                xr[3]=-1;
            }
            tg[0]=kni[row+1][j]-kni[row][j];
            tg[1]=hb[row+1][j]-hb[row][j];
            tg[2]=kr[row+1][j]-kr[row][j];
            tg[3]=gt[row+1][j]-gt[row][j];
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
    fprintf(f_para,"beta:\n%f",beta);
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
    fscanf(fp,"beta:\n%lf",&beta);
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
//    ln_rate-=d_rate;
}//para_update
*/
