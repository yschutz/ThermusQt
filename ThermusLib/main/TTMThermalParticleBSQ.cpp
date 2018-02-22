// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Thermal Particle class based on the complete grand-canonical approach.

#include <math.h>
#include <gsl/gsl_sf_bessel.h>

#include <QMessageBox>
#include <QScopedPointer>

#include "particlesdbmanager.h"
#include "f1.h"
#include "f2.h"
#include "FncsThermalModel.h"

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
    updateMembers();
}

//__________________________________________________________________________
TTMThermalParticleBSQ::TTMThermalParticleBSQ(TTMThermalParticleBSQ& obj) : TTMThermalParticle(obj.parent())
{
    // copy ctor
    mParameters = obj.mParameters;
    mCorrFactor = obj.mCorrFactor;
    mParticle   = obj.mParticle;
    updateMembers();
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::densityQStatNoWidth(double exclVolPressure)
{
    // Primordial Particle density assuming no width and Quantum stats
    //

    updateMembers(exclVolPressure);
    double lDensity = 0;

    int stat    = ParticlesDBManager::instance().getStat(mParticle);
    double mass = ParticlesDBManager::instance().getMass(mParticle);

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
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText("T=0 analytical solution requires Quantum stats");
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
double TTMThermalParticleBSQ::densityQStatWidth(double exclVolPressure)
{
    // Primordial Particle density assuming finite width and Quantum stats

    updateMembers(exclVolPressure);
    double lDensity = 0.0;

    int stat         = ParticlesDBManager::instance().getStat(mParticle);
    double width     = ParticlesDBManager::instance().getWidth(mParticle);
    double threshold = ParticlesDBManager::instance().getThreshold(mParticle);

    if (stat == 0)
        lDensity = densityBoltzmannWidth();
    else if (!parametersAllowed())
        lDensity = 0;
    else {
        if (width != 0) {
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnDensWidth)(double*, double*);
            pFcnDensWidth = &FcnDensWidth;
            QScopedPointer<F2> fn(new F2("n QStat width", pFcnDensWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fn->setParameters(mMu/mT, mM/mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", pFcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
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
double TTMThermalParticleBSQ::energyQStatNoWidth(double exclVolPressure)
{
    // Primordial Energy density contribution assuming no width and
    // Quantum stats

    updateMembers(exclVolPressure);
    double lEnergy = 0.0;

    int stat    = ParticlesDBManager::instance().getStat(mParticle);
    double mass = ParticlesDBManager::instance().getMass(mParticle);

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
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText("T=0 analytical solution requires Quantum stats");
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
            QScopedPointer<F1> fe(new F1("e QStat No width", pFcnEnergyDens, 0., 300., 5));
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
double TTMThermalParticleBSQ::energyQStatWidth(double exclVolPressure)
{
    // Primordial Energy density contribution assuming finite width and
    // Quantum stats

    updateMembers(exclVolPressure);
    double lEnergy = 0.0;

    int stat         = ParticlesDBManager::instance().getStat(mParticle);
    double width     = ParticlesDBManager::instance().getWidth(mParticle);
    double threshold = ParticlesDBManager::instance().getThreshold(mParticle);

    if (stat == 0.)
        lEnergy = energyBoltzmannWidth();
    else if (!parametersAllowed())
        lEnergy = 0;
    else {
        if (width != 0) {
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnEnergyDensWidth)(double*, double*);
            pFcnEnergyDensWidth = &FcnEnergyDensWidth;
            QScopedPointer<F2> fe(new F2("e QStat width", pFcnEnergyDensWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fe->setParameters(mMu / mT, mM / mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", pFcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
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
double TTMThermalParticleBSQ::entropyBoltzmannNoWidth(double exclVolPressure)
{
    // Primordial Entropy density contribution assuming no width and
    // Boltzmann stats

    updateMembers(exclVolPressure);

    return 1. / (2. * qPow(M_PI, 2)) * mG * mDeg * qPow(mM, 2) *
            mT * ((4. - mMu / mT) * gsl_sf_bessel_Kn(2, mM / mT) + mM / mT *
                  gsl_sf_bessel_Kn(1, mM / mT)) * qExp(mMu / mT) / kHC3;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::entropyBoltzmannWidth(double exclVolPressure)
{
    // Primordial Entropy density contribution assuming finite width and
    // Boltzmann stats

    updateMembers(exclVolPressure);
    double lEntropy  = 0.0;
    double width     = ParticlesDBManager::instance().getWidth(mParticle);

    if (width != 0) {
        double threshold = ParticlesDBManager::instance().getThreshold(mParticle);
        double a = qMax(mM - 2. * width, threshold);

        double (*pFcnEntropyBoltzmannWidth)(double*, double*);
        pFcnEntropyBoltzmannWidth = &FcnEntropyBoltzmannWidth;
        QScopedPointer<F1> fs(new F1("s Boltzmann Width", pFcnEntropyBoltzmannWidth, 0., (mM + 3. * width) / mT, 5));
        fs->setParameters(mMu / mT, mM / mT, mG, mDeg, width / mT);

        double s = IntegrateLegendre40(fs.data(), a / mT, (mM + 2. * width) / mT);

        double (*pFcnDensNormWidth)(double*, double*);
        pFcnDensNormWidth = &FcnDensNormWidth;
        QScopedPointer<F1> fnorm(new F1("norm", pFcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
        fnorm->setParameters(mM / mT, width / mT);

        double norm = IntegrateLegendre40(fnorm.data(), a / mT, (mM + 2. * width) / mT);

        lEntropy = mCorrFactor * qPow(mT, 3.) * s / norm / kHC3;
    } else
        lEntropy = entropyBoltzmannNoWidth();

    return lEntropy;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::entropyQStatNoWidth(double exclVolPressure)
{
    // Primordial Entropy density contribution assuming no width and
    // Quantum stats

    updateMembers(exclVolPressure);
    double lEntropy = 0;

    int stat = ParticlesDBManager::instance().getStat(mParticle);

    if (!parametersAllowed())
        lEntropy = 0;
    else if(stat == 0)
        lEntropy = entropyBoltzmannNoWidth();
    else {
        double (*pFcnEntropyDens)(double*, double*);
        pFcnEntropyDens = &FcnEntropyDens;
        QScopedPointer<F1> fs(new F1("s QStat No width", pFcnEntropyDens, 0., 300., 5));
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
double TTMThermalParticleBSQ::entropyQStatWidth(double exclVolPressure)
{
    // Primordial Entropy density contribution assuming finite width and
    // Quantum stats

    updateMembers(exclVolPressure);
    double lEntropy = 0;
    int stat        = ParticlesDBManager::instance().getStat(mParticle);

    if (stat == 0.)
        lEntropy = entropyBoltzmannWidth();
    else if (!parametersAllowed())
        lEntropy = 0;
    else {
        double width = ParticlesDBManager::instance().getWidth(mParticle);
        if (width != 0) {
            double threshold = ParticlesDBManager::instance().getThreshold(mParticle);
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnEntropyDensWidth)(double*, double*);
            pFcnEntropyDensWidth = &FcnEntropyDensWidth;
            QScopedPointer<F2> fs(new F2("s QStat width", pFcnEntropyDensWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fs->setParameters(mMu / mT, mM / mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", pFcnDensNormWidth, 0., (mM + 3. * width) / mT, 2));
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

    if (ParticlesDBManager::instance().getStat(mParticle) == -1) {
        updateMembers();
        if (qExp((mM - mMu) / mT) > mG) {
            return true;
        } else {
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(QString("Bose-Einstein Condensation of %1 (%2): mM=%3; mMu=%4; mT=%5; mG=%6").
                                   arg(ParticlesDBManager::instance().getName(mParticle)).
                                   arg(mParticle).
                                   arg(mM).arg(mMu).arg(mT).arg(mG));
            msg.exec();
            return false;
        }
    } else
        return true;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::pressureQStatNoWidth(double exclVolPressure)
{
    // Pressure contribution assuming no width and Quantum stats

    updateMembers(exclVolPressure);
    double lPressure = 0.;
    int stat         = ParticlesDBManager::instance().getStat(mParticle);

    if (!parametersAllowed())
        lPressure = 0;
    else if (stat == 0)
        lPressure = pressureBoltzmannNoWidth();
    else {
        double (*pFcnPressure)(double*, double*);
        pFcnPressure = &FcnPressure;
        QScopedPointer<F1> fP(new F1("P QStat No width", pFcnPressure, 0., 300., 5));
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
double TTMThermalParticleBSQ::pressureQStatWidth(double exclVolPressure)
{
    // Pressure contribution assuming finite width and Quantum stats

    updateMembers(exclVolPressure);
    double lPressure = 0;
    int stat         = ParticlesDBManager::instance().getStat(mParticle);

    if (stat == 0.)
        lPressure = pressureBoltzmannWidth();
    else if (!parametersAllowed())
        lPressure = 0;
    else {
        double width = ParticlesDBManager::instance().getWidth(mParticle);
        if (width != 0) {
            double threshold = ParticlesDBManager::instance().getThreshold(mParticle);
            double a = qMax(mM - 2. * width, threshold);

            double (*pFcnPressureWidth)(double*, double*);
            pFcnPressureWidth = &FcnPressureWidth;
            QScopedPointer<F2> fP(new F2("P QStat width", pFcnPressureWidth, 0., 300., 0., (mM + 3. * width) / mT, 6));
            fP->setParameters(mMu / mT, mM / mT, mG, stat, mDeg, width / mT);

            double (*pFcnDensNormWidth)(double*, double*);
            pFcnDensNormWidth = &FcnDensNormWidth;
            QScopedPointer<F1> fnorm(new F1("norm", pFcnDensNormWidth, 0., (mM + 3. * width) / mT,2));
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
void TTMThermalParticleBSQ::updateMembers(double exclVolPressure)
{
    mDeg    = ParticlesDBManager::instance().getSpin(mParticle);
    mM      = ParticlesDBManager::instance().getMass(mParticle);
    mB      = ParticlesDBManager::instance().getBaryon(mParticle);
    mS      = ParticlesDBManager::instance().getS(mParticle);
    mQ      = ParticlesDBManager::instance().getCharge(mParticle);
    mC      = ParticlesDBManager::instance().getCharm(mParticle);
    mBeauty = ParticlesDBManager::instance().getBeauty(mParticle);

    mT        = mParameters->getT();
    mMuB      = mParameters->getMuB();
    mMuS      = mParameters->getMuS();
    mMuQ      = mParameters->getMuQ();
    mMuC      = mParameters->getMuC();
    mMuBeauty = mParameters->getMuBeauty();

    mMu = mB * mMuB + mS * mMuS + mQ * mMuQ + mC * mMuC + mBeauty * mMuBeauty;

    double SContent = ParticlesDBManager::instance().getSContent(mParticle);
    double gammas = mParameters->getGammas();
    double Gs;

    double CContent = ParticlesDBManager::instance().getCContent(mParticle);
    double gammac = mParameters->getGammac();
    double Gc;

    double bContent = ParticlesDBManager::instance().getBContent(mParticle);
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

    if (exclVolPressure != 0.) {
        //         sfifts the chemical potential by an amount - (hadron volume) * P.
        mB      = 1;
        mS      = 0;
        mQ      = 0;
        mC      = 0;
        mBeauty = 0;
        double radius =   ParticlesDBManager::instance().getRadius(mParticle);
        double volume = 4. * 4./3. * M_PI * qPow(radius,3);
        double shift = volume * exclVolPressure;
        mMuB      = mMu - shift;
        mMuS      = 0.0;
        mMuQ      = 0.0;
        mMuC      = 0.0;
        mMuBeauty = 0.0;
    }
}

