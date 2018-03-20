#include<stdio.h>
int main()
{
    int i,j;
    j=0;
    for(i=0;i<20;i++)
    {
        printf("j=%d\t",j);
        if((i%2==0)&&(j++))
            printf("i=%d",i);
        printf("\n");
    }
}
