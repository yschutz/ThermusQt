// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Strangeness Canonical thermal model class.


#include <gsl/gsl_sf_bessel.h>

#include "TTMParameterSetBQ.h";
#include "TTMThermalModelBQ.h"
#include "functions/FncsConstrain.h"

#include <math.h>
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

    if( gsl_sf_bessel_In(n, x[1]) == 0. || gsl_sf_bessel_In(m, x[2]) == 0.) {
        t[0] = 0.;
        t[1] = 0.;
        t[2] = 0.;
        t[3] = 0.;
        t[4] = 0.;
        t[5] = 0.;
        t[6] = 0.;
    } else {
        double b = log10(gsl_sf_bessel_In(n, x[1]) / gsl_sf_bessel_In(0, x[1]));

    }
}
