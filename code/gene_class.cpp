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
    int i,j;
    beta = 0.02 * (rand()/2147483647.0) ;
    for(i=0;i<7;i++)
    {
        k[i]=2.0*(rand()/2147483647.0);
        for(j=0;j<4;j++)
        {
            v[i][j]=20.0*(rand()/2147483647.0)-10.0;
        }
    }
}

double SynBox::test(const char type[2],double smaller)
{
    char outkni[50];
    char outhb[50];
    char outkr[50];
    char outgt[50];

    sprintf(outkni,"output/%s/outkni.txt",type);
    sprintf(outhb,"output/%s/outhb.txt",type);
    sprintf(outkr,"output/%s/outkr.txt",type);
    sprintf(outgt,"output/%s/outgt.txt",type);
    

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
                fprintf(fokni,"%f\t",kni[j]);
                fprintf(fohb,"%f\t",hb[j]);
                fprintf(fokr,"%f\t",kr[j]);
                fprintf(fogt,"%f\t",gt[j]);
            }
            kni[j]+=(dkni[j]/smaller);
            hb[j]+=(dhb[j]/smaller);
            kr[j]+=(dkr[j]/smaller);
            gt[j]+=(dgt[j]/smaller);
        }
        if((i%(int)smaller)==0)
        {
            fprintf(fokni,"\n");
            fprintf(fohb,"\n");
            fprintf(fokr,"\n");
            fprintf(fogt,"\n");
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
    
    fclose(fokni);
    fclose(fohb);
    fclose(fokr);
    fclose(fogt);
//    printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n",k[0],k[1],k[2],k[3],k[4],k[5],k[6]);
    return(curve_err);
}


void SynBox::train(const char type[2],int step)
{
    FILE *ferr;
    ferr=fopen("err.txt","w");
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
        for(kk=0;kk<10*Nx;kk++)
        {
            j=(int)((rand()/2147483647.0)*Nx);
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
            para_update();
    //        printf("%.10f\n",loss_error);
        }///loop(j) 0 to 100 in one row
    //    if(i%100==0)
    //    {
            //printf("%d:",i);
            //test(type);
    //    }
        //fprintf(ferr,"%f\n",test(type));
        ln_rate-=d_rate;
    }//loop step(i)
    FILE *f_para;
    f_para=fopen("para/para.txt","w");
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
}
void SynBox::set(const char file[])
{
    printf("loading data from \"%s\"...\n",file);
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
    printf("set finished!\n\n");
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
}


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
    tmp=sqrt(tmp);
    for(i=0;i<7;i++)
    {
        n_ek[i]=n_ek[i]/tmp;
        for(j=0;j<4;j++)
            n_ev[i][j]=n_ev[i][j]/tmp;
    }
    n_eb=n_eb/tmp;
}
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
            tmp=D*(y1r[i]-y1[i]);
        else if(y1r[i]==-1)
            tmp=D*(y1l[i]-y1[i]);
        else
            tmp=D*(y1l[i]+y1r[i]-2*y1[i]);
        y5[i]+=tmp;
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
