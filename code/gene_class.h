#ifndef GeneClass
#define GeneClass

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define Nx 100
#define Nt 50//40//50
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
        void trainCurve(const char type[2],int step);
        void weightTrainCurve(const char type[2],int step);
        double test(const char type[2],double smaller);
        double testPredicError(const char type[2]);
        void predic(double x[7],double xl[4],double xr[4]);
        void set(const char para_file[]);

        void savePara(int dataNO);
        void saveTestResult(const char type[2],int dataNO,double curve_err);
        void savePredicError(const char type[2],int dataNO,double predicErr);
        double out[4];
    private:
        double testCurveErrorInFrame(int frame);
        double runCurveWithData(double smaller);
        void runCurve(double kni0[Nx],double hb0[Nx],double kr0[Nx],double gt0[Nx],double bcd[Nx],double nos[Nx],double tll[Nx],double smaller);

        double test_kni[Nt][Nx];
        double test_hb[Nt][Nx];
        double test_kr[Nt][Nx];
        double test_gt[Nt][Nx];
        void clearTestOut();
        void mkTestFile(const char type[2],int dataNO,double curve_err);

        double predic_error[Nt][Nx];
        void clearPredicError();

        void readData(const char type[2]);
        double kniData[Nt][Nx],hbData[Nt][Nx],krData[Nt][Nx],gtData[Nt][Nx],bcdData[Nx],nosData[Nx],tllData[Nx];


        void updateErrWeight(double raw_curve_error);
        double weightErr();
        double rawCurveErrorDist[4][Nx];
        double curveErrorWeight[4][Nx];



        double k[7];
        double v[7][4];
        double beta;
        double D;
        double dt;//=0.0001;

        double y1[7];
        double y1l[4];
        double y1r[4];
        double y2[7];
        double y3[7];
        double y4[4];
        double y5[4];
        double diff_f[4];///!!!!!

        double tg[4];

        double ln_rate;//=0.5;
        double d_rate;
        double h;//=5.0;

        double n_ek[7];
        double n_ev[7][4];
        double n_eb;
        double n_eD;        ///!!!
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
        double dedD();          ///!!!!
        void err();
        void para_update();
};
#endif


