#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
    int maxNO;
    sscanf(argv[1],"%d",&maxNO);
    int i,m,n;
    FILE *fp=NULL;
    FILE *fv=NULL;
    FILE *fk=NULL;
    char file[50];
    double k[7];
    double v[7][4];
    double beta;
    fv=fopen("code_clust/clust_v.txt","w");
    fk=fopen("code_clust/clust_k.txt","w");
    fprintf(fv,"#\t");
    fprintf(fv,"kni>kni\tkni>hb\tkni>kr\tkni>gt\t");
    fprintf(fv,"hb>kni\thb>hb\thb>kr\thb>gt\t");
    fprintf(fv,"kr>kni\tkr>hb\tkr>kr\tkr>gt\t");
    fprintf(fv,"gt>kni\tgt>hb\tgt>kr\tgt>gt\t");
    fprintf(fv,"Bcd>kni\tBcd>hb\tBcd>kr\tBcd>gt\t");
    fprintf(fv,"Nos>kni\tNos>hb\tNos>kr\tNos>gt\t");
    fprintf(fv,"Tll>kni\tTll>hb\tTll>kr\tTll>gt\t\n");
    fprintf(fk,"#\tkni\thb\tkr\tgt\tBcd\tNos\tTll\t\n");
    for(i=0;i<maxNO;i++)
    {
        sprintf(file,"para/para%d.txt",i);
        printf("opening %s\n",file);
        fp=fopen(file,"r");
        if(fp==NULL)
            continue;
        else
        {
            fprintf(fv,"%d\t",i);
            fprintf(fk,"%d\t",i);
            printf("OPEN success!\n");
            fscanf(fp,"k:\n");
            for(m=0;m<7;m++)
                fscanf(fp,"%lf\t",&k[m]);
            fscanf(fp,"v:\n");
            for(m=0;m<7;m++)
            {
                for(n=0;n<4;n++)
                    fscanf(fp,"%lf\t",&v[m][n]);
                fscanf(fp,"\n");
            }
            fscanf(fp,"beta:\n%lf",&beta);
            fclose(fp);
            fp=NULL;
            for(m=0;m<7;m++)
            {
                for(n=0;n<4;n++)
                    fprintf(fv,"%f\t",v[m][n]);
                fprintf(fk,"%f\t",k[m]);
            }
            fprintf(fv,"\n");
            fprintf(fk,"\n");
        }
    }
    fclose(fv);
    fclose(fk);
    return 0;
}
