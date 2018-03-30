#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;
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
