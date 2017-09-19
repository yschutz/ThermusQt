// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Base class for thermal particles

#include "gsl/gsl_sf_bessel.h"

#include <QScopedPointer>
#include <QtMath>

#include "external/f1.h"
#include "external/particlesdbmanager.h"
#include "functions/FncsThermalModel.h"

#include "TTMThermalParticle.h"

const double kHC3 = qPow(0.197, 3.);
//__________________________________________________________________________
TTMThermalParticle::TTMThermalParticle(QObject *parent) : QObject(parent)
{

}

//__________________________________________________________________________
double TTMThermalParticle::densityBoltzmannNoWidth()
{
    // Primordial Particle density assuming no width and Boltzmann stats

    updateMembers();

    return mCorrFactor / (2. * qPow(M_PI, 2)) * mG * mDeg * qPow(mM, 2) *
            mT * gsl_sf_bessel_Kn(2, mM / mT) * qExp(mMu / mT) / kHC3;
}

//__________________________________________________________________________
double TTMThermalParticle::densityBoltzmannWidth()
{
    // Primordial Particle density assuming finite width and Boltzmann stats

    updateMembers();
    double lDensity = 0.0;
    double width = ParticlesDBManager::Instance().getWidth(mParticle);

    if (width != 0) {

        double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);

        double a = qMax(mM - 2. * width, threshold);


        double (*pFcnDensBoltzmannWidth)(double*, double*);
        pFcnDensBoltzmannWidth = &FcnDensBoltzmannWidth;
        QScopedPointer<F1> fn(new F1("n Boltzmann Width", pFcnDensBoltzmannWidth, 0., (mM + 3. * width) / mT, 5));
        fn->setParameters(mMu / mT, mM / mT, mG, mDeg, width / mT);

        double n = IntegrateLegendre40(fn.data(), a / mT, (mM + 2. * width) / mT);

        double (*pFcnDensNormWidth)(double*, double*);
        pFcnDensNormWidth = &FcnDensNormWidth;
        QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
        fnorm->setParameters(mM / mT, width / mT);

        double norm = IntegrateLegendre40(fnorm.data(), a / mT, (mM + 2. * width) / mT);

        lDensity = mCorrFactor * qPow(mT, 3.) * n / norm / kHC3;
    } else
        lDensity = densityBoltzmannNoWidth();
    return lDensity;
}

//__________________________________________________________________________
double TTMThermalParticle::energyBoltzmannNoWidth()
{
    // Primordial Energy density contribution assuming no width and
    // Boltzmann stats

    updateMembers();

    return mCorrFactor / (2. * qPow(M_PI, 2)) * mG * mDeg * qPow(mM, 3)
            * mT * qExp(mMu / mT) * (gsl_sf_bessel_Kn(1, mM / mT) + 3. * mT / mM *
                                     gsl_sf_bessel_Kn(2, mM / mT)) / kHC3;
}

//__________________________________________________________________________
double TTMThermalParticle::energyBoltzmannWidth()
{
    // Primordial Energy density contribution assuming finite width and
    // Boltzmann stats
    //

    updateMembers();
    double lEnergy = 0;
    double width = ParticlesDBManager::Instance().getWidth(mParticle);

    if(width != 0){

        double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);

        double a = qMax(mM - 2. * width, threshold);

        double (*pFcnEnergyBoltzmannWidth)(double*, double*);
        pFcnEnergyBoltzmannWidth = &FcnEnergyBoltzmannWidth;
        QScopedPointer<F1> fe(new F1("e Boltzmann Width", FcnEnergyBoltzmannWidth, 0., (mM + 3. * width) / mT, 5));
        fe->setParameters(mMu / mT, mM / mT, mG, mDeg, width / mT);

        double e = IntegrateLegendre40(fe.data(), a / mT, (mM + 2. * width) / mT);

        double (*pFcnDensNormWidth)(double*, double*);
        pFcnDensNormWidth = &FcnDensNormWidth;
        QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
        fnorm->setParameters(mM / mT, width / mT);

        double norm = IntegrateLegendre40(fnorm.data(), a / mT, (mM + 2. * width) / mT);

        lEnergy = mCorrFactor * qPow(mT, 4.) * e / norm / kHC3;
    } else
        lEnergy = energyBoltzmannNoWidth();

    return lEnergy;
}

//__________________________________________________________________________
double TTMThermalParticle::pressureBoltzmannNoWidth()
{
    // Pressure contribution assuming no width and Boltzmann stats

    updateMembers();

    double dens = densityBoltzmannNoWidth();

    return dens * mT ;
}

//__________________________________________________________________________
double TTMThermalParticle::pressureBoltzmannWidth()
{
    // Pressure contribution assuming finite width and Boltzmann stat

    updateMembers();

    double dens = densityBoltzmannWidth();

    return dens * mT;
}
