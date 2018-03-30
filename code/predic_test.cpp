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
    int i,j,row;
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

    for(i=0;i<Nt-1;i++)
    {
        row=i;
        for(j=0;j<Nx;j++)
        {
            y1[0]=kni[row][j];
            y1[1]=hb[row][j];
            y1[2]=kr[row][j];
            y1[3]=gt[row][j];
            y1[4]=bcd[j];
            y1[5]=nos[j];
            y1[6]=tll[j];
            if(j!=0)
            {
                y1l[0]=kni[row][j-1];
                y1l[1]=hb[row][j-1];
                y1l[2]=kr[row][j-1];
                y1l[3]=gt[row][j-1];
            }
            else if(j==0)
            {
                y1l[0]=-1;
                y1l[1]=-1;
                y1l[2]=-1;
                y1l[3]=-1;
            }
            if(j!=Nx-1)
            {
                y1r[0]=kni[row][j+1];
                y1r[1]=hb[row][j+1];
                y1r[2]=kr[row][j+1];
                y1r[3]=gt[row][j+1];
            }
            else if(j==Nx-1)
            {
                y1r[0]=-1;
                y1r[1]=-1;
                y1r[2]=-1;
                y1r[3]=-1;
            }
            tg[0]=kni[row+1][j]-kni[row][j];
            tg[1]=hb[row+1][j]-hb[row][j];
            tg[2]=kr[row+1][j]-kr[row][j];
            tg[3]=gt[row+1][j]-gt[row][j];
            F();
            de();
            err();
	        predic_error[row][j]=loss_error;
            total_predic_error+=loss_error;
        }///loop(Nx) 0 to 100 in one row
    }//loop step(Nt)
    //printf("total predic error of %s : %f\n",type,total_predic_error);
    return(total_predic_error);
}//testPredicError

void SynBox::savePredicError(const char type[2],int dataNO,double predicErr)
{
    FILE *f_predicErr;
    char predicErrFile[50];
    int i,j;
    sprintf(predicErrFile,"predicError/%s%d(%.2f).txt",type,dataNO,predicErr);
    f_predicErr=fopen(predicErrFile,"w");
    for(i=0;i<Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fprintf(f_predicErr,"%f\t",predic_error[i][j]);
        }
        fprintf(f_predicErr,"\n");
    }
}
