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
    readData(type);
    double curve_err=runCurveWithData(smaller);
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
