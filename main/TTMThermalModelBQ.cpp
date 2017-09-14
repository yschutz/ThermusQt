// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Strangeness Canonical thermal model class.


#include <gsl/gsl_sf_bessel.h>

#include <QtMath>

#include "TTMDensObj.h"
#include "TTMParameterSetBQ.h"
#include "TTMThermalModelBQ.h"
#include "functions/FncsConstrain.h"

const double kLN10 = qLn(10.0);
//__________________________________________________________________________
TTMThermalModelBQ::TTMThermalModelBQ(QObject *parent) : TTMThermalModel(parent),
    mNonStrangeQStats(true), mPar(nullptr)
{
    //ctor
    mDescriptor = "SCanonical";
}

//__________________________________________________________________________
TTMThermalModelBQ::TTMThermalModelBQ(TTMParameterSetBQ *parameters, bool width, QObject *parent) : TTMThermalModel(parent),
    mNonStrangeQStats(true)
{
    mDescriptor = "SCanonical";
    mPar   = parameters;
    mWidth = width;
}

//__________________________________________________________________________
int TTMThermalModelBQ::generateParticleDens()
{
    // Calculates the Primordial particle densities and populates the density
    // hash table after first constraining muQ if required. The
    // Wroblewski factor and the decay contributions are also calculated
    // provided the decays have been entered into the particle set through
    // TTMParticleSet::InputDecays().

    int check = 1;

    if(mPar->getCorrRConstrain()) {
        mPar->setCanRadius(mPar->getRadius());
        mPar->getParameter(TTMParameterSet::kCRADIUS)->setStatus("(Set to Fireball Radius)");
    }
//    if(mPar->getMuQConstrain()) {

//    }
    return check;

}

//__________________________________________________________________________
int TTMThermalModelBQ::PrimPartDens()
{
    // Calculates the primordial particle densities and populates the density
    // hash table. The parameters are not constrained first!. This is the
    // function used by GenerateParticleDens() and calls Term().
    // Returns 1 if there are problems 0 else.

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();
}

//__________________________________________________________________________
void TTMThermalModelBQ::Term(double *x, double *y, uint m, uint n, double *t)
{
    // Private fcn which calculates normalised terms for the summations used
    // to calculate the correction factors
    //
    // 	t[0] : term for full partition function sum
    // 	t[1] : term for +1 sum
    // 	t[2] : term for +2 sum
    // 	t[3] : term for +3 sum
    // 	t[4] : term for -1 sum
    // 	t[5] : term for -2 sum
    // 	t[6] : term for -3 sum

    double S = mPar->getS();

    // if b and/or c are zero, then all are zero //

    if (gsl_sf_bessel_In(n, x[1]) == 0. || gsl_sf_bessel_In(m, x[2]) == 0.) {
        t[0] = 0.;
        t[1] = 0.;
        t[2] = 0.;
        t[3] = 0.;
        t[4] = 0.;
        t[5] = 0.;
        t[6] = 0.;
    } else {
        double b = qLn(gsl_sf_bessel_In(n, x[1]) / gsl_sf_bessel_In(0, x[1])) / kLN10;
        double c = qLn(gsl_sf_bessel_In(m, x[2]) / gsl_sf_bessel_In(0, x[2])) / kLN10;
        double d = m * qLn(y[2] / qPow(y[0], 3)) / kLN10;
        double e = n * qLn(y[1] / qPow(y[0], 2)) / kLN10;

        int nmS = qAbs(3 * m + 2 * n - S);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[0] = 0.0;
        } else {
            double a0 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[0] = qPow(10., a0 + b + c + d + e + S * qLn(y[0]) / kLN10);
        }

        nmS = qAbs(3 * m + 2 * n - S + 1);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[1] = 0.;
        } else {
            double ap1 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[1] = qPow(10., ap1 + b + c + d + e + (S - 1.) * qLn(y[0]) / kLN10);
        }

        nmS = qAbs(3 * m + 2 * n - S + 2);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[2] = 0.;
        } else {
            double ap2 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[2] = qPow(10., ap2 + b + c + d + e + (S - 2.) * qLn(y[0]) / kLN10);
        }

        nmS = qAbs(3 * m + 2 * n - S + 3);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[3] = 0.;
        } else {
            double ap3 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[3] = qPow(10., ap3 + b + c + d + e + (S - 3.) * qLn(y[0]) / kLN10);
        }

        nmS = qAbs(3 * m + 2 * n - S - 1);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[4] = 0.;
        } else {
            double am1 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[4] = qPow(10., am1 + b + c + d + e + (S + 1.) * qLn(y[0]) / kLN10);
        }

        nmS = qAbs(3 * m + 2 * n - S - 2);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[5] = 0.;
        } else {
            double am2 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[5] = qPow(10., am2 + b + c + d + e + (S + 2.) * qLn(y[0]) / kLN10);
        }

        nmS = qAbs(3 * m + 2 * n - S - 3);
        if (gsl_sf_bessel_In(nmS, x[0]) == 0.) {
            t[6] = 0.;
        } else {
            double am3 = qLn(gsl_sf_bessel_In(nmS, x[0]) / gsl_sf_bessel_In(0, x[0])) / kLN10;
            t[6] = qPow(10., am3 + b + c + d + e + (S + 3.) * qLn(y[0]) / kLN10);
        }
    }
}
