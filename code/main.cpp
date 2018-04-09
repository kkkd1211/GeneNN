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
    double errRE,predicErr;
    sscanf(argv[1],"%d",&dataNO);
    srand((unsigned)((unsigned)time(NULL)*(1+dataNO)+dataNO));
    SynBox box;
    box.train("my",1000000);
    //box.trainCurve("my",10000);
    predicErr=box.testPredicError("my");
            box.savePredicError("my",dataNO,predicErr);
    printf("total predic error : %f\n",predicErr);
    if(1)
    {
        box.savePara(dataNO);
        errRE=box.test("my",100.0);
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

