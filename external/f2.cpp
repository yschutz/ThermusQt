// a class to mimic basic features of root TF
// Y. Schutz Septembre 2017

#include <QMessageBox>
#include <QScopedPointer>
#include <QtMath>

#include "f2.h"

//F2::F2()
//{

//}

//__________________________________________________________________________
F2::F2(const QString &name, double (*fcn)(double *, double *), double xmin, double xmax, double ymin, double ymax, int par, QObject *parent) :
    F1(name, fcn, xmax, xmin, par, parent), mYmax(ymax), mYMin(ymin)
{

}

//__________________________________________________________________________
double F2::eval(double x, double y)
{
    // evalutes the function for x, y
    QScopedPointer<double>xx(new double[2]);
    xx.data()[0] = x;
    xx.data()[1] = y;
    return mFunction(xx.data(), mPar);
}

//__________________________________________________________________________
double F2::integral(double ax, double bx, double ay, double by, double epsrel)
{
    //   integrates the function between (ax, ay) and (bx, by)
    //   from Root::AdaptiveIntegratorMultiDim::DoIntegral
    //     References:
    //
    //           1.A.C. Genz and A.A. Malik, Remarks on algorithm 006:
    //             An adaptive algorithm for numerical integration over
    //             an N-dimensional rectangular region, J. Comput. Appl. Math. 6 (1980) 295-302.
    //           2.A. van Doren and L. de Ridder, An adaptive algorithm for numerical
    //             integration over an n-dimensional cube, J.Comput. Appl. Math. 2 (1976) 207-217.

    const int n   = 2;
    double epsabs = epsrel;

    double ctr[n], wth[n], wthl[n], z[n];

    const double xl2 = 0.358568582800318073; // lambda_2
    const double xl4 = 0.948683298050513796; // lambda_4
    const double xl5 = 0.688247201611685289; // lambda_5
    const double w2  = 980./6561;            // weights/2^n
    const double w4  = 200./19683;
    const double wp2 = 245./486;             // error weights/2^n
    const double wp4 = 25./729;

    const double wn1[14]  = { -0.193872885230909911, -0.555606360818980835,
                              -0.876695625666819078, -1.15714067977442459,  -1.39694152314179743,
                              -1.59609815576893754,  -1.75461057765584494,  -1.87247878880251983,
                              -1.94970278920896201,  -1.98628257887517146,  -1.98221815780114818,
                              -1.93750952598689219,  -1.85215668343240347,  -1.72615963013768225};

    const double wn3[14]  = {  0.0518213686937966768,  0.0314992633236803330,
                               0.0111771579535639891,-0.00914494741655235473,-0.0294670527866686986,
                              -0.0497891581567850424,-0.0701112635269013768, -0.0904333688970177241,
                              -0.110755474267134071, -0.131077579637250419,  -0.151399685007366752,
                              -0.171721790377483099, -0.192043895747599447,  -0.212366001117715794};

    const double wn5[14]  = {  0.871183254585174982e-01,  0.435591627292587508e-01,
                               0.217795813646293754e-01,  0.108897906823146873e-01,  0.544489534115734364e-02,
                               0.272244767057867193e-02,  0.136122383528933596e-02,  0.680611917644667955e-03,
                               0.340305958822333977e-03,  0.170152979411166995e-03,  0.850764897055834977e-04,
                               0.425382448527917472e-04,  0.212691224263958736e-04,  0.106345612131979372e-04};

    const double wpn1[14] = { -1.33196159122085045, -2.29218106995884763,
                              -3.11522633744855959, -3.80109739368998611, -4.34979423868312742,
                              -4.76131687242798352, -5.03566529492455417, -5.17283950617283939,
                              -5.17283950617283939, -5.03566529492455417, -4.76131687242798352,
                              -4.34979423868312742, -3.80109739368998611, -3.11522633744855959};

    const double wpn3[14] = {  0.0445816186556927292, -0.0240054869684499309,
                              -0.0925925925925925875, -0.161179698216735251,  -0.229766803840877915,
                              -0.298353909465020564,  -0.366941015089163228,  -0.435528120713305891,
                              -0.504115226337448555,  -0.572702331961591218,  -0.641289437585733882,
                              -0.709876543209876532,  -0.778463648834019195,  -0.847050754458161859};

    double result  = 0;
    double abserr  = 0;
    int    status  = 3;
//    int    nfnevl  = 0;
    int    relerr  = 0;
    double twondm  = qPow(2.0, n);
    int    ifncls  = 0;
    bool   ldv     = false;
    int    irgnst  = 2 * n + 3;
    int    irlcls  = twondm + 2 * n * (n + 1) + 1;//minimal number of nodes in n dim
    int    isbrgn  = irgnst;
    int    isbrgs  = irgnst;
    int    maxpts  = irlcls ;//specified maximal number of function evaluations
    int    minpts  = irlcls;

    if (maxpts < minpts)
        maxpts = 10 * minpts;

    int    iwk     = irgnst * (1 + maxpts / irlcls) / 2;
    double *wk     = new double[iwk + 10];

    ctr[0] = (bx + ax) * 0.5; //center of a hypercube
    wth[0] = (bx - ax) * 0.5; //its width
    ctr[1] = (by + ay) * 0.5; //center of a hypercube
    wth[1] = (by - ay) * 0.5; //its width
L20:
    double rgnvol = twondm; //=2^n
    for (int j = 0; j < n; j++) {
        rgnvol *= wth[j]; //region volume
        z[j]    = ctr[j]; //temporary node
    }
    double sum1   = eval(z[0], z[1]);
    int  idvaxn   = 0, idvax0 = 0, isbtmp, isbtpp;
    double difmax = 0.0;
    double sum2   = 0.0;
    double sum3   = 0.0;

    //loop over coordinates
    for (int j = 0; j < n; j++) {
        z[j]    = ctr[j] - xl2 * wth[j];
        double f2 = eval(z[0], z[1]);
        z[j]    = ctr[j] + xl2 * wth[j];
        f2 += eval(z[0], z[1]);
        wthl[j] = xl4 * wth[j];
        z[j]    = ctr[j] - wthl[j];
        double f3 = eval(z[0], z[1]);
        z[j]    = ctr[j] + wthl[j];
        f3 += eval(z[0], z[1]);
        sum2   += f2;                  // sum func eval with different weights separately
        sum3   += f3;                  // for a given region
        double dif = qAbs(7 * f2 - f3 - 12 * sum1);
        //storing dimension with biggest error/difference (?)
        if (dif >= difmax) {
            difmax = dif;
            idvaxn = j + 1;
        }
        z[j] = ctr[j];
    }

    double sum4 = 0;
    for (int j = 1; j < n; j++) {
        int j1 = j - 1;
        for (int k = j; k < n; k++) {
            for (int l = 0; l < 2; l++) {
                wthl[j1] = -wthl[j1];
                z[j1]    = ctr[j1] + wthl[j1];
                for (int m = 0; m < 2; m++) {
                    wthl[k] = -wthl[k];
                    z[k]    = ctr[k] + wthl[k];
                    sum4 += eval(z[0], z[1]);
                }
            }
            z[k] = ctr[k];
        }
        z[j1] = ctr[j1];
    }

    double sum5 = 0;

    for (int j = 0; j < n; j++) {
        wthl[j] = -xl5 * wth[j];
        z[j] = ctr[j] + wthl[j];
    }

L90:                            //sum over end nodes ~gray codes
    sum5 += eval(z[0], z[1]);
    for (int j = 0;j < n; j++) {
        wthl[j] = -wthl[j];
        z[j] = ctr[j] + wthl[j];
        if (wthl[j] > 0) goto L90;
    }

    double rgncmp  = rgnvol * (wpn1[n-2] * sum1 + wp2 * sum2 + wpn3[n-2] * sum3 + wp4 * sum4);
    double rgnval  = wn1[n-2] * sum1 + w2 * sum2 + wn3[n-2] * sum3 + w4 * sum4 + wn5[n-2] * sum5;
    rgnval *= rgnvol;
    double rgnerr  = qAbs(rgnval - rgncmp);  //   compares estim error with expected error

    result += rgnval;
    abserr += rgnerr;
    ifncls += irlcls;
    double aresult = qAbs(result);

    //if division
    if (ldv) {
L110:
        isbtmp = 2 * isbrgn;
        if (isbtmp > isbrgs) goto L160;
        if (isbtmp < isbrgs) {
            isbtpp = isbtmp + irgnst;
            if (wk[isbtmp - 1] < wk[isbtpp - 1])
                isbtmp = isbtpp;
        }
        if (rgnerr >= wk[isbtmp-1]) goto L160;
        for (int k = 0; k < irgnst; k++) {
            wk[isbrgn - k - 1] = wk[isbtmp - k - 1];
        }
        isbrgn = isbtmp;
        goto L110;
    }

L140:
    isbtmp = (isbrgn / (2 * irgnst)) * irgnst;
    if (isbtmp >= irgnst && rgnerr > wk[isbtmp-1]) {
        for (int k = 0; k < irgnst; k++) {
            wk[isbrgn - k - 1] = wk[isbtmp - k - 1];
        }
        isbrgn = isbtmp;
        goto L140;
    }

L160: //to divide or not
    wk[isbrgn - 1] = rgnerr;//storing value & error in last
    wk[isbrgn - 2] = rgnval;//table records
    wk[isbrgn - 3] = double(idvaxn);//coordinate with biggest error
    for (int j = 0; j < n; j++) {
        isbtmp = isbrgn - 2 * j - 4;
        wk[isbtmp]   = ctr[j];
        wk[isbtmp-1] = wth[j];
    }
    if (ldv) {//divison along chosen coordinate
        ldv = false;
        ctr[idvax0 - 1] += 2 * wth[idvax0 - 1];
        isbrgs += irgnst;//updating the number of nodes/regions(?)
        isbrgn  = isbrgs;
        goto L20;
    }
    //if no divisions to be made..
    relerr = abserr;
    if (aresult != 0)
        relerr = abserr / aresult;
    if (relerr < 1e-1 && aresult < 1e-20)
        status = 0;
    if (relerr < 1e-3 && aresult < 1e-10)
        status = 0;
    if (relerr < 1e-5 && aresult < 1e-5)
        status = 0;
    if (isbrgs+irgnst > iwk)
        status = 2;
    if (ifncls + 2 * irlcls > maxpts) {
        if (sum1 == 0 && sum2 == 0 && sum3 == 0 && sum4 == 0 && sum5 == 0){
            status = 0;
            result = 0;
        }       else
            status = 1;
    }
    //..and accuracy appropriare
    // should not use absolute tolerance especially for sharp peaks
    if ( ( relerr < epsrel || abserr < epsabs ) && ifncls >= minpts)
        status = 0;

    if (status == 3) {
        ldv     = true;
        isbrgn  = irgnst;
        abserr -= wk[isbrgn - 1];
        result -= wk[isbrgn-2];
        idvax0  = (unsigned int)(wk[isbrgn-3]);
        for (int j = 0;j < n; j++) {
            isbtmp = isbrgn - 2 * j - 4;
            ctr[j] = wk[isbtmp];
            wth[j] = wk[isbtmp - 1];
        }
        if (idvax0 < 1) {
            // Can happen for overflows / degenerate floats.
            idvax0 = 1;
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "Logic error: idvax0 < 1!");
            msg.exec();
        }
        wth[idvax0 - 1]  = 0.5 * wth[idvax0 - 1];
        ctr[idvax0 - 1] -= wth[idvax0 - 1];
        goto L20;
    }
//    nfnevl = ifncls;       //number of function evaluations performed.
    delete [] wk;

    return result;         //an approximate value of the integral
}
