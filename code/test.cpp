#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;

void SynBox::clearTestOut()
{
    int i,j;
    for(i=0;i<Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            test_kni[i][j]=0;
            test_hb[i][j]=0;
            test_kr[i][j]=0;
            test_gt[i][j]=0;
        }
    }
}
double SynBox::test(const char type[2],double smaller)
{
    clearTestOut();
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
    int i,j,ii;
    fkni=fopen(filekni,"r");
    fhb=fopen(filehb,"r");
    fkr=fopen(filekr,"r");
    fgt=fopen(filegt,"r");
    ii=0;
    do
    {
        ii++;
        for(j=0;j<Nx;j++)
        {
            fscanf(fkni,"%lf ",&kni[j]);
            fscanf(fhb,"%lf ",&hb[j]);
            fscanf(fkr,"%lf ",&kr[j]);
            fscanf(fgt,"%lf ",&gt[j]);
        }
    }while(kni[0]!=kni[0]);
    for(i=0;i<Nt-ii;i++)
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
    for(i=0;i<Nt*smaller;i++)
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
        }//loop j 0 to 100
        for(j=0;j<Nx;j++)
        {
            if((i%(int)smaller)==0)
            {
                test_kni[(int)(i/smaller)][j]=kni[j];
                test_hb[(int)(i/smaller)][j]=hb[j];
                test_kr[(int)(i/smaller)][j]=kr[j];
                test_gt[(int)(i/smaller)][j]=gt[j];
            }
            kni[j]+=(dkni[j]/smaller);
            hb[j]+=(dhb[j]/smaller);
            kr[j]+=(dkr[j]/smaller);
            gt[j]+=(dgt[j]/smaller);
        }
    }//loop i 0 to 60
    double curve_err=0;
    for(j=0;j<Nx;j++)
    {
        curve_err+=(kni[j]-tg_kni[j])*(kni[j]-tg_kni[j]);
        curve_err+=(hb[j]-tg_hb[j])*(hb[j]-tg_hb[j]);
        curve_err+=(kr[j]-tg_kr[j])*(kr[j]-tg_kr[j]);
        curve_err+=(gt[j]-tg_gt[j])*(gt[j]-tg_gt[j]);
    }
    curve_err*=0.5;
    printf("curve total error (%s) : %f\n",type,curve_err);
    return(curve_err);
}//test
void SynBox::mkTestFile(const char type[2],int dataNO,double curve_err)
{
    char file[50];
    sprintf(file,"testoutput/data%d",dataNO);
    printf("\tmkdir:%s...",file);
    mkdir(file,0777);
    printf("done!\n");
    sprintf(file,"testoutput/data%d/%s(%.2f)",dataNO,type,curve_err);
    printf("\tmkdir:%s...",file);
    mkdir(file,0777);
    printf("done!\n");
}//mkTestFile
void SynBox::saveTestResult(const char type[2],int dataNO,double curve_err)
{
    mkTestFile(type,dataNO,curve_err);
    FILE *f_kni,*f_hb,*f_kr,*f_gt;
    char knifile[50];
    char hbfile[50];
    char krfile[50];
    char gtfile[50];
    sprintf(knifile,"testoutput/data%d/%s(%.2f)/outkni.txt",dataNO,type,curve_err);
    sprintf(hbfile,"testoutput/data%d/%s(%.2f)/outhb.txt",dataNO,type,curve_err);
    sprintf(krfile,"testoutput/data%d/%s(%.2f)/outkr.txt",dataNO,type,curve_err);
    sprintf(gtfile,"testoutput/data%d/%s(%.2f)/outgt.txt",dataNO,type,curve_err);
    f_kni=fopen(knifile,"w");
    f_hb=fopen(hbfile,"w");
    f_kr=fopen(krfile,"w");
    f_gt=fopen(gtfile,"w");
    int i,j;
    for(i=0;i<Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fprintf(f_kni,"%f\t",test_kni[i][j]);
            fprintf(f_hb,"%f\t",test_hb[i][j]);
            fprintf(f_kr,"%f\t",test_kr[i][j]);
            fprintf(f_gt,"%f\t",test_gt[i][j]);
        }
        fprintf(f_kni,"\n");
        fprintf(f_hb,"\n");
        fprintf(f_kr,"\n");
        fprintf(f_gt,"\n");
    }
    fclose(f_kni);
    fclose(f_hb);
    fclose(f_kr);
    fclose(f_gt);
}//saveTestResult