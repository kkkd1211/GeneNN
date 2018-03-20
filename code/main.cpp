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
    box.test("wt");
    box.train("wt",100000);
    box.test("wt");
}

