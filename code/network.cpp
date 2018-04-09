#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;
double SynBox::testCurveErrorInFrame(int frame)
{
    double tmp=0;
    int j;
    for(j=0;j<Nx;j++)
    {
        rawCurveErrorDist[0][j]=0.5*(test_kni[frame][j]-kniData[frame][j])*(test_kni[frame][j]-kniData[frame][j]);
        rawCurveErrorDist[1][j]=0.5*(test_hb[frame][j]-hbData[frame][j])*(test_hb[frame][j]-hbData[frame][j]);
        rawCurveErrorDist[2][j]=0.5*(test_kr[frame][j]-krData[frame][j])*(test_kr[frame][j]-krData[frame][j]);
        rawCurveErrorDist[3][j]=0.5*(test_gt[frame][j]-gtData[frame][j])*(test_gt[frame][j]-gtData[frame][j]);
        tmp=tmp+rawCurveErrorDist[0][j]+rawCurveErrorDist[1][j]+rawCurveErrorDist[2][j]+rawCurveErrorDist[3][j];
    }
    return tmp;
}
double SynBox::runCurveWithData(double smaller)
{
    runCurve(kniData[0],hbData[0],krData[0],gtData[0],bcdData,nosData,tllData,smaller);
    int j;
    double curve_err=testCurveErrorInFrame(Nt-1);
//    curve_err+=testCurveErrorInFrame(19);
//    curve_err/=2.0;
    return(curve_err);
}
void SynBox::runCurve(double kni0[Nx],double hb0[Nx],double kr0[Nx],double gt0[Nx],double bcd[Nx],double nos[Nx],double tll[Nx],double smaller)
{
    int i,j;
    double x[7],xl[4],xr[4];
    double kni[Nx],hb[Nx],kr[Nx],gt[Nx];
    double dkni[Nx],dhb[Nx],dkr[Nx],dgt[Nx];
    for(j=0;j<Nx;j++)
    {
        kni[j]=kni0[j];
        hb[j]=hb0[j];
        kr[j]=kr0[j];
        gt[j]=gt0[j];
    }
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
        }//loop j 0 to Nx
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
}
void SynBox::readData(const char type[2])
{
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
    fkni=fopen(filekni,"r");
    fhb=fopen(filehb,"r");
    fkr=fopen(filekr,"r");
    fgt=fopen(filegt,"r");
    fbcd=fopen(filebcd,"r");
    fnos=fopen(filenos,"r");
    ftll=fopen(filetll,"r");

    int i,j;
    for(i=0;i<Nt;i++)
    {
        for(j=0;j<Nx;j++)
        {
            fscanf(fkni,"%lf ",&kniData[i][j]);
            fscanf(fhb,"%lf ",&hbData[i][j]);
            fscanf(fkr,"%lf ",&krData[i][j]);
            fscanf(fgt,"%lf ",&gtData[i][j]);
        }
    }
    for(i=0;i<Nx;i++)
    {
        fscanf(fbcd,"%lf ",&bcdData[i]);
        fscanf(fnos,"%lf ",&nosData[i]);
        fscanf(ftll,"%lf ",&tllData[i]);
    }


    fclose(fkni);
    fclose(fhb);
    fclose(fkr);
    fclose(fgt);
    fclose(fbcd);
    fclose(fnos);
    fclose(ftll);
}
void SynBox::predic(double x[7],double xl[4],double xr[4])
{
    int i;
    for(i=0;i<4;i++)
    {
        y1l[i]=xl[i];
        y1r[i]=xr[i];
    }
    for(i=0;i<7;i++)
        y1[i]=x[i];
    F();
}
void SynBox::de()
{
    int i,j;
    double tmp=0;
    for(i=0;i<7;i++)
    {
        n_ek[i]=dedk(i);
        tmp+=n_ek[i]*n_ek[i];
        for(j=0;j<4;j++)
        {
            n_ev[i][j]=dedv(i,j);
            tmp+=n_ev[i][j]*n_ev[i][j];
        }
    }
    n_eb=dedb();
    tmp+=n_eb*n_eb;
    n_eD=dedD();
    tmp+=n_eD*n_eD;
    tmp=sqrt(tmp);
    for(i=0;i<7;i++)
    {
        n_ek[i]=n_ek[i]/tmp;
        for(j=0;j<4;j++)
            n_ev[i][j]=n_ev[i][j]/tmp;
    }
    n_eb=n_eb/tmp;
    n_eD=n_eD/tmp;
}
double SynBox::dedD()
{
    double tmp=0;
    int i;
    for(i=0;i<4;i++)
    {
        tmp+=(out[i]-tg[i])*diff_f[i];
    }
    tmp *= dt;
    return tmp;
}//dedD()
double SynBox::dedv(int i,int j)
{
    if((i>=7)||(j>=4))
    {
        printf("dedv:i,j err!");
        exit(1);
    }
    return(dt * (out[j]-tg[j]) * y3[i] );
}
double SynBox::dedk(int i)
{
    double tmp=0;
    int a;
    for(a=0;a<4;a++)
    {
        tmp+=(out[a]-tg[a])*v[i][a];
    }
    tmp = tmp * dt * (1-y3[i]) * (1-y3[i]) * y1[i];
    return tmp;
}
double SynBox::dedb()
{
    double tmp=0;
    int i;
    for(i=0;i<4;i++)
    {
        tmp-=(out[i]-tg[i])*y1[i];
    }
    tmp *= dt;
    return tmp;
}
void SynBox::err()
{
    loss_error=0;
    int i;
    for(i=0;i<4;i++)
    {
        loss_error+=(out[i]-tg[i])*(out[i]-tg[i]);
    }
    loss_error*=0.5;
}
void SynBox::F()
{
    f12();
    f23();
    f34();
    f45();
    f_out();
}
void SynBox::f12()
{
    int i;
    for(i=0;i<7;i++)
        y2[i]=y1[i]*k[i];
}
void SynBox::f23()
{
    int i;
    for(i=0;i<7;i++)
        y3[i]=y2[i]/(1.0+y2[i]);
}
void SynBox::f34()
{
    int i,j;
    for(j=0;j<4;j++)
    {
        y4[j]=0;
        for(i=0;i<7;i++)
            y4[j]+=y3[i]*v[i][j];
    }
}
void SynBox::f45()
{
    int i;
    double tmp;
    for(i=0;i<4;i++)
    {
        y5[i]=y4[i]-beta*y1[i];
        if(y1l[i]==-1)
            diff_f[i]=(y1r[i]-y1[i]);
        else if(y1r[i]==-1)
            diff_f[i]=(y1l[i]-y1[i]);
        else
            diff_f[i]=(y1l[i]+y1r[i]-2*y1[i]);
        y5[i]+=diff_f[i]*D;
        y5[i]*=dt;
        if((y1[i]+y5[i]<0))
            y5[i]=-y1[i];
    }
}
void SynBox::f_out()
{
    int i;
    for(i=0;i<4;i++)
    {
        out[i]=y5[i];
    }
}
