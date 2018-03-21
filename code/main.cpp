#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "gene_class.h"
using namespace std;

int main()
{
    int i;
    srand((unsigned)time(NULL));
    SynBox box; 
    box.train("my",100000);
    //box.set("para/para1.txt");
    box.test("my",100.0);
    box.test("wt",100.0);
    box.test("B+",100.0);
    box.test("B-",100.0);
    box.test("N+",100.0);
    box.test("N-",100.0);
    box.test("T-",100.0);
}

