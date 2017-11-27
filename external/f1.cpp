// a class to mimic basic features of root TF1
// Y. Schutz Septembre 2017

#include <QMessageBox>
#include "f1.h"

//__________________________________________________________________________
F1::F1(const QString &name, double (*fcn)(double *, double *), double xmin, double xmax, int par, QObject *parent) : QObject(parent),
    mFunction(fcn), mXMax(xmax), mXMin(xmin), mNPar(par)
{
    // ctor
    setObjectName(name);
    mPar = new double[mNPar];

}

//__________________________________________________________________________
F1::~F1()
{
    // dtor
    if (mPar)
        delete [] mPar;
}

//__________________________________________________________________________
double F1::eval(double x)
{
    // evalutes the function for x
    double* xx = new double[1];
    xx[0] = x;
    double rv = mFunction(xx, mPar);
    delete[] xx;
    return rv;
}

//__________________________________________________________________________
double F1::integral(double a, double b, double epsrel)
{
    // Return Integral of function between a and b.
    const double kHF = 0.5;
    const double kCST = 5./1000;
    double x[12] = { 0.96028985649753623,  0.79666647741362674,
                     0.52553240991632899,  0.18343464249564980,
                     0.98940093499164993,  0.94457502307323258,
                     0.86563120238783174,  0.75540440835500303,
                     0.61787624440264375,  0.45801677765722739,
                     0.28160355077925891,  0.09501250983763744};

    double w[12] = { 0.10122853629037626,  0.22238103445337447,
                     0.31370664587788729,  0.36268378337836198,
                     0.02715245941175409,  0.06225352393864789,
                     0.09515851168249278,  0.12462897125553387,
                     0.14959598881657673,  0.16915651939500254,
                     0.18260341504492359,  0.18945061045506850};

    double h = 0;
    if (b == a)
        return h;
    double aconst = kCST / qAbs(b - a);
    double bb = a;
CASE1:
    double aa = bb;
    bb = b;
CASE2:
    double c1 = kHF * (bb + aa);
    double c2 = kHF * (bb - aa);
    double s8 = 0.0;
    for (int i = 0; i < 4; i++) {
        double u  = c2 * x[i];
        double xx = c1 + u;
        double f1 = eval(xx);
        xx        = c1 - u;
        double f2 = eval(xx);
        s8   += w[i] * (f1 + f2);
    }
    double s16 = 0;
    for (int i = 4; i < 12; i++) {
        double u  = c2 * x[i];
        double xx = c1 +u;
        double f1 = eval(xx);
        xx        = c1 - u;
        double f2 = eval(xx);
        s16  += w[i] * (f1 + f2);
    }
    s16 = c2 * s16;
    double error = qAbs(s16 - c2 * s8);
    if (error <= epsrel * qAbs(s16)) {
        h += s16;
        if (bb != b)
            goto CASE1;
    } else {
        bb = c1;
        if (1. + aconst * qAbs(c2) != 1)
            goto CASE2;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, QString("Failed to reach the desired tolerance %1").arg(epsrel));
        msg.exec();
        h = s8;  //this is a crude approximation (cernlib function returned 0 !)
    }
    error = qAbs(s16 - c2 * s8);

    return h;
}

//__________________________________________________________________________
void F1::setParameters(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8, double p9, double p10)
{
    // set the parameters value
    if (mNPar >=  1) setParameter(0,  p0);
    if (mNPar >=  2) setParameter(1,  p1);
    if (mNPar >=  3) setParameter(2,  p2);
    if (mNPar >=  4) setParameter(3,  p3);
    if (mNPar >=  5) setParameter(4,  p4);
    if (mNPar >=  6) setParameter(5,  p5);
    if (mNPar >=  7) setParameter(6,  p6);
    if (mNPar >=  8) setParameter(7,  p7);
    if (mNPar >=  9) setParameter(8,  p8);
    if (mNPar >= 10) setParameter(9,  p9);
    if (mNPar >= 11) setParameter(10, p10);
}
