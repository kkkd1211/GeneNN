#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;
void SynBox::train_curve(const char type[2],int step)
{
    clearTestOut();
    readData(type);
    h=10.0;
    ln_rate=0.5;
    d_rate=0.4/step;
    int a,b,i;
    double delta;
    double v0[7][4],k0[7],beta0,D0;
    double curve_err0,curve_err;
    double tmp;
    for(i=0;i<step;i++)
    {
        curve_err0=runCurveWithData(1.0);
        for(a=0;a<7;a++)
        {
            for(b=0;b<4;b++)
                v0[a][b]=v[a][b];
            k0[a]=k[a];
        }
        beta0=beta;
		D0=D;
        tmp=0;
        for(a=0;a<7;a++)
        {
            for(b=0;b<4;b++)
            {
                delta=0.00001*(0.1+fabs(v[a][b]));
                v[a][b]+=delta;
                curve_err=runCurveWithData(1.0);
                n_ev[a][b]=(curve_err-curve_err0)/delta;
                v[a][b]=v0[a][b];
                tmp+=n_ev[a][b]*n_ev[a][b];
            }
            delta=0.00001*(0.1+fabs(k[a]));
            k[a]+=delta;
            curve_err=runCurveWithData(1.0);
            n_ek[a]=(curve_err-curve_err0)/delta;
            k[a]=k0[a];
            tmp+=n_ek[a]*n_ek[a];
        }
        delta=0.00001*(0.1+fabs(beta));
        beta+=delta;
        curve_err=runCurveWithData(1.0);
        n_eb=(curve_err-curve_err0)/delta;
        beta=beta0;
        tmp+=n_eb*n_eb;

        delta=0.00001*(0.1+fabs(D));
        D+=delta;
        curve_err=runCurveWithData(1.0);
        n_eD=(curve_err-curve_err0)/delta;
        D=D0;
        tmp+=n_eD*n_eD;
        if(tmp==0)
        {
            printf("\n=============\nFINISH at step %d\n=================\n",i);
            break;
        }
        tmp=sqrt(tmp);
        for(a=0;a<7;a++)
        {
            for(b=0;b<4;b++)
                n_ev[a][b]=n_ev[a][b]/tmp;
            n_ek[a]=n_ek[a]/tmp;
        }
        n_eb=n_eb/tmp;
		n_eD=n_eD/tmp;
        para_update();
        ln_rate-=d_rate;
    }//loop i=0:step
}//train_curve
