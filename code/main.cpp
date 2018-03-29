#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;

int main(int argc,char *argv[])
{
    printf("\n\n======Start!========\n\n");
    int dataNO;
    double errRE;
    sscanf(argv[1],"%d",&dataNO);
    srand((unsigned)((unsigned)time(NULL)*dataNO+dataNO));
    SynBox box; 
    box.train("wt",100000);
    //box.savePara(dataNO);
    //box.set("para/para1.txt");
    errRE=box.test("my",100.0);
    if(1)
    {
        box.savePara(dataNO);
        		box.saveTestResult("my",dataNO,errRE);
        errRE=box.test("wt",100.0);
                box.saveTestResult("wt",dataNO,errRE);
        errRE=box.test("B+",100.0);
                box.saveTestResult("B+",dataNO,errRE);
        errRE=box.test("B-",100.0);
                box.saveTestResult("B-",dataNO,errRE);
        errRE=box.test("N+",100.0);
                box.saveTestResult("N+",dataNO,errRE);
        errRE=box.test("N-",100.0);
                box.saveTestResult("N-",dataNO,errRE);
        errRE=box.test("T-",100.0);
                box.saveTestResult("T-",dataNO,errRE);
    }
}

