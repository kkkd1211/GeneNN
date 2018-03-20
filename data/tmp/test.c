#include <stdio.h>

int main()
{
    FILE *p;
    p=fopen("hb.txt","r");
    int i=0;
    int j=0;
    double tmp;
    while(!feof(p))
    {
        fscanf(p,"%lf ",tmp);
        if(tmp==tmp)
            printf("%f",tmp);
        else
            j++;
        i++;
    }
    printf("\n%d:%d\n",i,j);
}
