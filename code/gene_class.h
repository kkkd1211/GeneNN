#ifndef GeneClass
#define GeneClass

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define Nx 100
#define Nt 50
#define k_epsilon 0.000001
#define kkk printf("kkk!!!\n");
using namespace std;
class SynBox;

class SynBox
{
    public:
//        ~SynBox();
        SynBox();
        void train(const char type[2],int step);
        double test(const char type[2],double smaller);
//        double test_smallstep(const char type[2]);
        void predic(double x[7],double xl[4],double xr[4]);
        void set(const char para_file[]);        

        double out[4];
    private:
        double k[7];
        double v[7][4];
        double beta;
        double D=1000;
        double dt=0.0001;
        
        double y1[7];
        double y1l[4];
        double y1r[4];
        double y2[7];
        double y3[7];
        double y4[4];
        double y5[4];
        
        double tg[4];

        double ln_rate=0.5;
        double d_rate;
        double h=5.0;

        double n_ek[7];
        double n_ev[7][4];
        double n_eb;
        double loss_error;

        void F();
        void f12();
        void f23();
        void f34();
        void f45();
        void f_out();
        void de();
        double dedk(int i);
        double dedv(int i,int j);
        double dedb();
        void err();
        void para_update();        
};
#endif


