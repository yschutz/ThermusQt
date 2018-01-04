// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Thermal Particle class based on the strangeness-canonical approach (i.e.
// strangeness canonical, baryon number and charge grand-canonical).
// The strangeness-canonical particle and energy densities and pressure in the
// Boltzmann approximation differ from those of the complete grand-
// canonical approach by a multiplicative factor calculated from the
// densities of all particles in the fireball. Thus at this level, this
// factor cannot be calculated, but rather has to be specified (fCorrFactor).
// Only when a particle set is specified can it be determined.
//

#include "external/particlesdbmanager.h"

#include "TTMParameterSetBQ.h"
#include "TTMThermalParticle.h"
#include "TTMThermalParticleBQ.h"

//__________________________________________________________________________
TTMThermalParticleBQ::TTMThermalParticleBQ(QObject *parent) : TTMThermalParticle(parent),
    mParameters(nullptr)
{
    //default ctor
     mCorrFactor = 1;
}

//__________________________________________________________________________
TTMThermalParticleBQ::TTMThermalParticleBQ(int part, TTMParameterSetBQ *parm, double correction, QObject *parent) :
    TTMThermalParticle(parent), mParameters(parm)
{
    // ctor
    mCorrFactor = correction;
    mParticle  = part;
}

//__________________________________________________________________________
TTMThermalParticleBQ::TTMThermalParticleBQ(TTMThermalParticleBQ &obj) : TTMThermalParticle(obj.parent())
{
    // copy ctor
    mParticle   = obj.getParticle();
    mParameters = obj.getParameters();
    mCorrFactor = obj.getCorrFactor();
    updateMembers();
}

//__________________________________________________________________________
void TTMThermalParticleBQ::updateMembers(double /*exclVolPressure*/)
{
    mDeg = ParticlesDBManager::instance().getSpin(mParticle);
    mM   = ParticlesDBManager::instance().getMass(mParticle);
    mT   = mParameters->getT();

    double B        = ParticlesDBManager::instance().getBaryon(mParticle);
    double Q        = ParticlesDBManager::instance().getCharge(mParticle);
    double SContent = ParticlesDBManager::instance().getSContent(mParticle);

    double muB    = mParameters->getMuB();
    double muQ    = mParameters->getMuQ();
    double gammas = mParameters->getGammas();

    mMu = B * muB + Q * muQ;

    if (SContent != 0.0)
      mG = qPow(gammas, SContent);
    else
        mG = 1.0;
}

//__________________________________________________________________________
TTMThermalParticleBQ &TTMThermalParticleBQ::operator=(TTMThermalParticleBQ &obj)
{
    // assign operator

    if (this == &obj) return *this;

    mParticle   = obj.getParticle();
    mParameters = obj.getParameters();
    mCorrFactor = obj.getCorrFactor();
    updateMembers();
    return *this;
}
