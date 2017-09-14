// a class to mimic basic features of root TF1
// Y. Schutz Septembre 2017

#include "f1.h"

//__________________________________________________________________________
//F1::F1(QObject *parent) : QObject(parent),
//    mFunction(nullptr)
//{

//}

//__________________________________________________________________________
F1::F1(const QString &name, double (*fcn)(double *, double *), double xmin, double xmax, int par, QObject *parent) : QObject(parent),
    mFunction(fcn), mMax(xmax), mMin(xmin), mNPar(par)
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
    return mFunction(xx, mPar);
}

//__________________________________________________________________________
double F1::integral(double a, double b, double epsrel)
{

}

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
