// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Thermal Particle class based on the complete grand-canonical approach.

#include <math.h>
#include <gsl/gsl_sf_bessel.h>

#include <QMessageBox>
#include <QScopedPointer>

#include "external/particlesdbmanager.h"
#include "external/f1.h"
#include "external/f2.h"
#include "functions/FncsThermalModel.h"

#include "TTMParameterSetBSQ.h"
#include "TTMThermalParticleBSQ.h"

const double kHC3 = qPow(0.197, 3.);
const double kEPS = kEPS;
//__________________________________________________________________________
TTMThermalParticleBSQ::TTMThermalParticleBSQ(QObject* parent) : TTMThermalParticle(parent),
    mParameters(nullptr)
{
    //default ctor
    mCorrFactor = 1.0;
    mParticle   = 0;
}

//__________________________________________________________________________
TTMThermalParticleBSQ::TTMThermalParticleBSQ(int part, TTMParameterSetBSQ *parm, QObject *parent) : TTMThermalParticle(parent),
    mParameters(parm)
{
    // ctor
    mCorrFactor = 1.0;
    mParticle   = part;
}

//__________________________________________________________________________
TTMThermalParticleBSQ::TTMThermalParticleBSQ(TTMThermalParticleBSQ& obj)
{
    // copy ctor
    mParameters = obj.mParameters;
    mCorrFactor = obj.mCorrFactor;
    mParticle   = obj.mParticle;
    updateMembers();
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::densityQStatNoWidth()
{
    // Primordial Particle density assuming no width and Quantum stats
    //

    updateMembers();
    double lDensity = 0;

    int stat    = ParticlesDBManager::Instance().getStat(mParticle);
    double mass = ParticlesDBManager::Instance().getMass(mParticle);

    if (mT==0.) {
        if (stat == -1.)
            lDensity = 0;
        else if (stat == 1.) {
            if (mMu > mM) {
                lDensity = 1. / (2. * qPow(M_PI, 2)) * mDeg * 1. / 3. *
                        qPow(mMu * mMu - mass * mass, 3./2.) / kHC3;
            } else
                lDensity = 0;
        } else {
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "T=0 analytical solution requires Quantum stats");
            msg.exec();
            lDensity = 0;
        }
    } else {
        if (!parametersAllowed()) {
            lDensity = 0;
        } else if (stat == 0)
            lDensity = densityBoltzmannNoWidth();
        else {
            double (*pFcnDens)(double*, double*);
            pFcnDens = &FcnDens;
            QScopedPointer<F1> fn(new F1("n QStat No width", pFcnDens, 0., 300., 5));
            fn->setParameters(mMu / mT, mM / mT, mG, stat, mDeg);

            if(stat == +1 && mMu >= mM){

                double guessn = IntegrateLegendre32(fn.data(), 0., 250.);
                double n = fn->integral(0., 250., guessn * kEPS);

                lDensity = qPow(mT, 3.) * n / kHC3;
            } else {
                double n = IntegrateLaguerre32(fn.data());
                lDensity = qPow(mT, 3.) * n / kHC3;
            }
        }
    }
    return lDensity;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::densityQStatWidth()
{
    // Primordial Particle density assuming finite width and Quantum stats

    updateMembers();
    double lDensity = 0.0;

    int stat         = ParticlesDBManager::Instance().getStat(mParticle);
    double width     = ParticlesDBManager::Instance().getWidth(mParticle);
    double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);

    if (stat == 0)
        lDensity = densityBoltzmannWidth();
    else if (!parametersAllowed())
        lDensity = 0;
    else {
        if (width != 0) {
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnDensWidth)(double*, double*);
            pFcnDensWidth = &FcnDensWidth;
            QScopedPointer<F2> fn(new F2("n QStat width", FcnDensWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fn->setParameters(mMu/mT, mM/mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
            fnorm->setParameters(mM / mT, width / mT);

            if(stat == +1 && mMu > mM){
                double guessn    = Integrate2DLaguerre32Legendre32(fn.data(), a / mT, (mM + 2. * width) / mT);
                double n         = fn->integral(0., 250., a / mT, (mM + 2. * width) / mT, guessn * kEPS);
                double guessnorm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);
                double norm      = fnorm->integral(a / mT,(mM + 2. * width) / mT, guessnorm * kEPS);

                lDensity = qPow(mT, 3.) * n / norm / kHC3;
            } else {
                double n    = Integrate2DLaguerre32Legendre32(fn.data(), a / mT, (mM + 2. * width) / mT);
                double norm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);

                lDensity = qPow(mT, 3.) * n / norm / kHC3;
            }
        } else
            lDensity = densityQStatNoWidth();
    }
    return lDensity;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::energyQStatNoWidth()
{
    // Primordial Energy density contribution assuming no width and
    // Quantum stats

    updateMembers();
    double lEnergy = 0.0;

    int stat    = ParticlesDBManager::Instance().getStat(mParticle);
    double mass = ParticlesDBManager::Instance().getMass(mParticle);

    if (mT==0.) {
        if(stat == -1.)
            lEnergy = 0;
        else if(stat == 1.) {
            if (mMu > mass) {
                double a = qSqrt(mMu * mMu - mass * mass) / mass + mMu / mass;
                lEnergy = 1. / (2. * qPow(M_PI,  2)) * mDeg * qPow(mass, 4.) *
                        (- 1. / 8. * qLn(a) + 1. / 32. * sinh(4. * qLn(a))) / kHC3;
            } else
                lEnergy = 0;
        } else {
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "T=0 analytical solution requires Quantum stats");
            msg.exec();
            lEnergy = 0;
        }
    } else {
        if (!parametersAllowed())
            lEnergy = 0;
        else if (stat == 0)
            lEnergy = energyBoltzmannNoWidth();
        else {
            double (*pFcnEnergyDens)(double*, double*);
            pFcnEnergyDens = &FcnEnergyDens;
            QScopedPointer<F1> fe(new F1("e QStat No width", FcnEnergyDens, 0., 300., 5));
            fe->setParameters(mMu / mT, mM / mT, mG, stat, mDeg);

            if (stat == +1 && mMu >= mM) {
                double guesse = IntegrateLegendre32(fe.data(), 0., 250.);
                double e      = fe->integral(0., 250., guesse * kEPS);
                lEnergy       = qPow(mT, 4.) * e / kHC3;
            } else {
                double e = IntegrateLaguerre32(fe.data());
                lEnergy  = qPow(mT, 4.) * e / kHC3;
            }
        }
    }
    return lEnergy;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::energyQStatWidth()
{
    // Primordial Energy density contribution assuming finite width and
    // Quantum stats

    updateMembers();
    double lEnergy = 0.0;

    int stat         = ParticlesDBManager::Instance().getStat(mParticle);
    double width     = ParticlesDBManager::Instance().getWidth(mParticle);
    double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);

    if (stat == 0.)
        lEnergy = energyBoltzmannWidth();
    else if (!parametersAllowed())
        lEnergy = 0;
    else {
        if (width != 0) {
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnEnergyDensWidth)(double*, double*);
            pFcnEnergyDensWidth = &FcnEnergyDensWidth;
            QScopedPointer<F2> fe(new F2("e QStat width", FcnEnergyDensWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fe->setParameters(mMu / mT, mM / mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
            fnorm->setParameters(mM / mT, width / mT);

            if (stat == +1 && mMu > mM) {
                double guesse    = Integrate2DLaguerre32Legendre32(fe.data(), a / mT, (mM + 2. * width) / mT);
                double e         = fe->integral(0., 250., a / mT, (mM + 2.* width) / mT, guesse * kEPS);
                double guessnorm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);
                double norm      = fnorm->integral(a / mT, (mM + 2. * width) / mT, guessnorm * kEPS);

                lEnergy          = qPow(mT, 4.) * e / norm / kHC3;
            } else {
                double e    = Integrate2DLaguerre32Legendre32(fe.data(), a / mT, (mM + 2. * width) / mT);
                double norm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);

                lEnergy     = qPow(mT, 4.) * e / norm / kHC3;
            }
        } else
            lEnergy = energyQStatNoWidth();
    }
    return lEnergy;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::entropyBoltzmannNoWidth()
{
    // Primordial Entropy density contribution assuming no width and
    // Boltzmann stats

    updateMembers();

    return 1. / (2. * qPow(M_PI, 2)) * mG * mDeg * qPow(mM, 2) *
            mT * ((4. - mMu / mT) * gsl_sf_bessel_Kn(2, mM / mT) + mM / mT *
                  gsl_sf_bessel_Kn(1, mM / mT)) * qExp(mMu / mT) / kHC3;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::entropyBoltzmannWidth()
{
    // Primordial Entropy density contribution assuming finite width and
    // Boltzmann stats

    updateMembers();
    double lEntropy  = 0.0;
    double width     = ParticlesDBManager::Instance().getWidth(mParticle);

    if (width != 0) {
        double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);
        double a = qMax(mM - 2. * width, threshold);

        double (*pFcnEntropyBoltzmannWidth)(double*, double*);
        pFcnEntropyBoltzmannWidth = &FcnEntropyBoltzmannWidth;
        QScopedPointer<F1> fs(new F1("s Boltzmann Width", FcnEntropyBoltzmannWidth, 0., (mM + 3. * width) / mT, 5));
        fs->setParameters(mMu / mT, mM / mT, mG, mDeg, width / mT);

        double s = IntegrateLegendre40(fs.data(), a / mT, (mM + 2. * width) / mT);

        double (*pFcnDensNormWidth)(double*, double*);
        pFcnDensNormWidth = &FcnDensNormWidth;
        QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
        fnorm->setParameters(mM / mT, width / mT);

        double norm = IntegrateLegendre40(fnorm.data(), a / mT, (mM + 2. * width) / mT);

        lEntropy = mCorrFactor * qPow(mT, 3.) * s / norm / kHC3;
    } else
        lEntropy = entropyBoltzmannNoWidth();

    return lEntropy;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::entropyQStatNoWidth()
{
    // Primordial Entropy density contribution assuming no width and
    // Quantum stats

    updateMembers();
    double lEntropy = 0;

    int stat = ParticlesDBManager::Instance().getStat(mParticle);

    if (!parametersAllowed())
        lEntropy = 0;
    else if(stat == 0)
        lEntropy = entropyBoltzmannNoWidth();
    else {
        double (*pFcnEntropyDens)(double*, double*);
        pFcnEntropyDens = &FcnEntropyDens;
        QScopedPointer<F1> fs(new F1("s QStat No width", FcnEntropyDens, 0., 300., 5));
        fs->setParameters(mMu / mT, mM / mT, mG, stat, mDeg);
        if (stat == +1 && mMu >= mM) {
            double guesss = IntegrateLegendre32(fs.data(), 0., 250.);
            double s      = fs->integral(0., 250., guesss * kEPS);
            lEntropy      = qPow(mT, 3.) * s / kHC3;
        } else {
            double s = IntegrateLaguerre32(fs.data());
            lEntropy = qPow(mT, 3.) * s / kHC3;
        }
    }
    return lEntropy;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::entropyQStatWidth()
{
    // Primordial Entropy density contribution assuming finite width and
    // Quantum stats

    updateMembers();
    double lEntropy = 0;
    int stat        = ParticlesDBManager::Instance().getStat(mParticle);

    if (stat == 0.)
        lEntropy = entropyBoltzmannWidth();
    else if (!parametersAllowed())
        lEntropy = 0;
    else {
        double width = ParticlesDBManager::Instance().getWidth(mParticle);
        if (width != 0) {
            double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnEntropyDensWidth)(double*, double*);
            pFcnEntropyDensWidth = &FcnEntropyDensWidth;
            QScopedPointer<F2> fs(new F2("s QStat width", FcnEntropyDensWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fs->setParameters(mMu / mT, mM / mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
            fnorm->setParameters(mM / mT, width / mT);

            if (stat == +1 && mMu > mM) {
                double guesss    = Integrate2DLaguerre32Legendre32(fs.data(), a / mT, (mM + 2. * width) / mT);
                double s         = fs->integral(0., 250., a / mT, (mM + 2. * width) / mT, guesss * kEPS);
                double guessnorm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);
                double norm      = fnorm->integral(a / mT, (mM + 2. * width) / mT, guessnorm * kEPS);

                lEntropy         = qPow(mT, 3.) * s / norm / kHC3;
            } else {
                double s    = Integrate2DLaguerre32Legendre32(fs.data(), a / mT, (mM + 2. * width) / mT);
                double norm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);

                lEntropy    = qPow(mT, 3.) * s / norm / kHC3;
            }
        } else
            lEntropy = entropyQStatNoWidth();
    }
    return lEntropy;
}

//__________________________________________________________________________
bool TTMThermalParticleBSQ::parametersAllowed()
{
    // Checks for Bosons that
    // e^{(m_i-mu_i)/T}>Gammas^{|S_i|}*Gammac^{|C_i|}*Gammab^{|b_i|}

    if (ParticlesDBManager::Instance().getStat(mParticle) == -1) {
        updateMembers();
        if (qExp((mM - mMu) / mT) > mG) {
            return true;
        } else {
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO,
                            QString("Bose-Einstein Condensation of %1").arg(ParticlesDBManager::Instance().getName(mParticle)));
            msg.exec();
            return false;
        }
    } else
        return true;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::pressureQStatNoWidth()
{
    // Pressure contribution assuming no width and Quantum stats

    updateMembers();
    double lPressure = 0.;
    int stat         = ParticlesDBManager::Instance().getStat(mParticle);

    if (!parametersAllowed())
        lPressure = 0;
    else if (stat == 0)
        lPressure = pressureBoltzmannNoWidth();
    else {
        double (*pFcnPressure)(double*, double*);
        pFcnPressure = &FcnPressure;
        QScopedPointer<F1> fP(new F1("P QStat No width", FcnPressure, 0., 300., 5));
        fP->setParameters(mMu / mT, mM / mT, mG, stat, mDeg);

        if(stat == +1 && mMu >= mM){
            double guessp = IntegrateLegendre32(fP.data(), 0., 250.);
            double P      = fP->integral(0., 250., guessp * kEPS);

            lPressure     = qPow(mT, 4.) * P / kHC3;
        } else {
            double P  = IntegrateLaguerre32(fP.data());
            lPressure = qPow(mT, 4.) * P / kHC3;
        }
    }
    return lPressure;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::pressureQStatWidth()
{
    // Pressure contribution assuming finite width and Quantum stats

    updateMembers();
    double lPressure = 0;
    int stat         = ParticlesDBManager::Instance().getStat(mParticle);

    if (stat == 0.)
        lPressure = pressureBoltzmannWidth();
    else if (!parametersAllowed())
        lPressure = 0;
    else {
        double width = ParticlesDBManager::Instance().getWidth(mParticle);
        if (width != 0) {
            double threshold = ParticlesDBManager::Instance().getThreshold(mParticle);
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnPressureWidth)(double*, double*);
            pFcnPressureWidth = &FcnPressureWidth;
            QScopedPointer<F2> fP(new F2("P QStat width", FcnPressureWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fP->setParameters(mMu / mT, mM / mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", FcnDensNormWidth, 0., (mM + 3. * width) / mT,2));
            fnorm->setParameters(mM / mT, width / mT);

            if (stat == +1 && mMu > mM) {
                double guessP = Integrate2DLaguerre32Legendre32(fP.data(), a / mT, (mM + 2. * width) / mT);
                double P         = fP->integral(0., 250., a / mT, (mM + 2. * width) / mT, guessP * kEPS);
                double guessnorm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);
                double norm      = fnorm->integral(a / mT, (mM + 2. * width) / mT, guessnorm * kEPS);

                lPressure        = qPow(mT, 4.) * P / norm / kHC3;
            } else {
                double P    = Integrate2DLaguerre32Legendre32(fP.data(), a / mT, (mM + 2. * width) / mT);
                double norm = IntegrateLegendre32(fnorm.data(), a / mT, (mM + 2. * width) / mT);

                lPressure   = qPow(mT, 4.) * P / norm / kHC3;
            }
        } else
            lPressure = pressureQStatNoWidth();
    }
    return lPressure;
}

//__________________________________________________________________________
void TTMThermalParticleBSQ::updateMembers()
{
    mDeg            = ParticlesDBManager::Instance().getSpin(mParticle);
    mM              = ParticlesDBManager::Instance().getMass(mParticle);
    double B        =  ParticlesDBManager::Instance().getBaryon(mParticle);
    double S        = ParticlesDBManager::Instance().getS(mParticle);
    double Q        = ParticlesDBManager::Instance().getCharge(mParticle);
    double C        = ParticlesDBManager::Instance().getCharm(mParticle);
    double b        = ParticlesDBManager::Instance().getBeauty(mParticle);

    mT = mParameters->getT();


    double muB = mParameters->getMuB();
    double muS = mParameters->getMuS();
    double muQ = mParameters->getMuQ();
    double muC = mParameters->getMuC();
    double mub = mParameters->getMuBeauty();

    mMu = B * muB + S * muS + Q * muQ + C * muC + b * mub;

    double SContent = ParticlesDBManager::Instance().getSContent(mParticle);
    double gammas = mParameters->getGammas();
    double Gs;

     double CContent = ParticlesDBManager::Instance().getCContent(mParticle);
     double gammac = mParameters->getGammac();
     double Gc;

     double bContent = ParticlesDBManager::Instance().getBContent(mParticle);
     double gammab = mParameters->getGammab();
     double Gb;

     if (SContent != 0.0)
       Gs = qPow(gammas, SContent);
     else
       Gs = 1.0;

     if (CContent != 0.0)
       Gc = qPow(gammac, CContent);
     else
       Gc = 1.0;

     if (bContent != 0.0)
       Gb = qPow(gammab, bContent);
     else
       Gb = 1.0;

     mG = Gs * Gc * Gb;
}

