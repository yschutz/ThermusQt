// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Thermal Particle class based on the complete grand-canonical approach.

#include <QMessageBox>

#include "external/particlesdbmanager.h"
#include "external/f1.h"
#include "functions/FncsThermalModel.h"

#include "TTMParameterSetBSQ.h"
#include "TTMThermalParticleBSQ.h"

const double kHC3 = qPow(0.197, 3.);

//__________________________________________________________________________
TTMThermalParticleBSQ::TTMThermalParticleBSQ(QObject* parent) : TTMThermalParticle(parent),
    mParameters(nullptr)
{
    //default ctor
    mCorrFactor = 1.0;
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

            F1 *fn = new F1("n QStat No width", pFcnDens, 0., 300., 5);
            fn->setParameters(mMu / mT, mM / mT, mG, stat, mDeg);

            if(stat == +1 && mMu >= mM){

                double guessn = IntegrateLegendre32(fn, 0., 250.);
                //double n = fn->Integral(0.,250.,(double *)0,guessn*1e-10);
                double n = fn->Integral(0., 250., guessn * 1e-10);

                lDensity = qPow(mT, 3.) * n / kHC3;
            } else {
                double n = IntegrateLaguerre32(fn);
                lDensity = qPow(mT, 3.) * n / kHC3;
            }
            delete fn; // getting rid of memory leak : BH 09/02/2016
        }
    }
    return lDensity;
}

//__________________________________________________________________________
double TTMThermalParticleBSQ::densityQStatWidth()
{

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
TTMThermalParticleBSQ::TTMThermalParticleBSQ(int part, TTMParameterSetBSQ *parm, QObject *parent) :
    TTMThermalParticle(parent), mParameters(parm)
{
    // ctor
    mCorrFactor = 1.0;
    mParticle = part;
}

//__________________________________________________________________________
TTMThermalParticleBSQ::TTMThermalParticleBSQ(TTMThermalParticleBSQ &obj)
{
    // assign operator
    mParticle = obj.getParticle();
    mParameters = obj.getParameters();
    mCorrFactor = obj.getCorrFactor();
    updateMembers();
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

