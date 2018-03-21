#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;
#define small_para 100
double SynBox::test_smallstep(const char type[2])
{
    char outkni[50];
    char outhb[50];
    char outkr[50];
    char outgt[50];

    sprintf(outkni,"outkni.txt");
    sprintf(outhb,"outhb.txt");
    sprintf(outkr,"outkr.txt");
    sprintf(outgt,"outgt.txt");
    

    FILE *fokni,*fohb,*fokr,*fogt;
    fokni=fopen(outkni,"w");
    fohb=fopen(outhb,"w");
    fokr=fopen(outkr,"w");
    fogt=fopen(outgt,"w");

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
    double kni[Nx],hb[Nx],kr[Nx],gt[Nx];
    double dkni[Nx],dhb[Nx],dkr[Nx],dgt[Nx];
    double tg_kni[Nx],tg_hb[Nx],tg_kr[Nx],tg_gt[Nx];
    double bcd[Nx],nos[Nx],tll[Nx];
    int i,j;
    fkni=fopen(filekni,"r");
    fhb=fopen(filehb,"r");
    fkr=fopen(filekr,"r");
    fgt=fopen(filegt,"r");
    for(i=0;i<2;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fscanf(fkni,"%lf ",&kni[j]);
            fscanf(fhb,"%lf ",&hb[j]);
            fscanf(fkr,"%lf ",&kr[j]);
            fscanf(fgt,"%lf ",&gt[j]);
        }
    }
    for(i=0;i<Nt-2;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fscanf(fkni,"%lf ",&tg_kni[j]);
            fscanf(fhb,"%lf ",&tg_hb[j]);
            fscanf(fkr,"%lf ",&tg_kr[j]);
            fscanf(fgt,"%lf ",&tg_gt[j]);
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
    double x[7],xl[4],xr[4];
    for(i=0;i<small_para*Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            x[0]=kni[j];
            x[1]=hb[j];
            x[2]=kr[j];
            x[3]=gt[j];
            x[4]=bcd[j];
            x[5]=nos[j];
            x[6]=tll[j];
            if(j==0)
            {
                xl[0]=-1;
                xl[1]=-1;
                xl[2]=-1;
                xl[3]=-1;
            }
            else if(j!=0)
            {
                xl[0]=kni[j-1];
                xl[1]=hb[j-1];
                xl[2]=kr[j-1];
                xl[3]=gt[j-1];
            }
            if(j==Nx-1)
            {
                xr[0]=-1;
                xr[1]=-1;
                xr[2]=-1;
                xr[3]=-1;
            }
            else if(j!=Nx-1)
            {
                xr[0]=kni[j+1];
                xr[1]=hb[j+1];
                xr[2]=kr[j+1];
                xr[3]=gt[j+1];
            }
            predic(x,xl,xr);
            dkni[j]=out[0];
            dhb[j]=out[1];
            dkr[j]=out[2];
            dgt[j]=out[3];
        }//loop j 0 to Nx
        for(j=0;j<Nx;j++)
        {
            if((i%small_para)==0)
            {   
                fprintf(fokni,"%f\t",kni[j]);
                fprintf(fohb,"%f\t",hb[j]);
                fprintf(fokr,"%f\t",kr[j]);
                fprintf(fogt,"%f\t",gt[j]);
            }
            kni[j]+=(dkni[j]/small_para);
            hb[j]+=(dhb[j]/small_para);
            kr[j]+=(dkr[j]/small_para);
            gt[j]+=(dgt[j]/small_para);
        }
        if((i%small_para==0))
        {
            fprintf(fokni,"\n");
            fprintf(fohb,"\n");
            fprintf(fokr,"\n");
            fprintf(fogt,"\n");
        }
    }//loop i 0 to Nt
    double curve_err=0;
    for(j=0;j<Nx;j++)
    {
        curve_err+=(kni[j]-tg_kni[j])*(kni[j]*tg_kni[j]);
        curve_err+=(hb[j]-tg_hb[j])*(hb[j]-tg_hb[j]);
        curve_err+=(kr[j]-tg_kr[j])*(kr[j]-tg_kr[j]);
        curve_err+=(gt[j]-tg_gt[j])*(gt[j]-tg_gt[j]);
    }
    curve_err*=0.5;
    printf("curve total error : %f\n",curve_err);
    
    fclose(fokni);
    fclose(fohb);
    fclose(fokr);
    fclose(fogt);
//    printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n",k[0],k[1],k[2],k[3],k[4],k[5],k[6]);
    return(curve_err);
}
