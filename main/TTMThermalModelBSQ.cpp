// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt Yves Schutz Octobre 2017
//__________________________________________________________________________
// Grand Canonical thermal model class.
//   

#include <QMessageBox>

#include "external/particlesdbmanager.h"
#include "functions/FncsConstrain.h"
#include "functions/FncsThermalModel.h"

#include "TTMDensObj.h"
#include "TTMThermalModelBSQ.h"

//__________________________________________________________________________
TTMThermalModelBSQ::TTMThermalModelBSQ(QObject* parent) : TTMThermalModel(parent),
    mExclVolCorrection(false), mExclVolDenominator(1.0), mExclVolPressure(0.0), mQStats(true)
{
    // default ctor
    mDescriptor = "GCanonical";
    mWidth      = true;
    mPar        = nullptr;
    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();

    mSplus   = mSminus = mBplus = mBminus = mQplus = mQminus = mCplus = mCminus = mbplus = mbminus = 0.;
    mStrange = mBaryon = mCharge = mDensity = mWroblewski = mCharm = mBeauty = 0.;
    mEnergy  = mEntropy = mPressure = 0.;
}

//__________________________________________________________________________
TTMThermalModelBSQ::TTMThermalModelBSQ(TTMParameterSetBSQ *parameters, bool qstats, bool width) :
    mExclVolCorrection(false), mExclVolDenominator(1.0), mExclVolPressure(0.0), mQStats(qstats)
{
    // ctor
    mDescriptor = "GCanonical";
    mWidth      = width;
    mPar        = parameters;
    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();

    mSplus   = mSminus = mBplus = mBminus = mQplus = mQminus = mCplus = mCminus = mbplus = mbminus = 0.;
    mStrange = mBaryon = mCharge = mDensity = mWroblewski = mCharm = mBeauty = 0.;
    mEnergy  = mEntropy = mPressure = 0.;
}

//__________________________________________________________________________
TTMThermalModelBSQ::TTMThermalModelBSQ(const TTMThermalModelBSQ &model)
{
    // copy ctor
    mExclVolCorrection  = model.mExclVolCorrection;
    mExclVolDenominator = model.mExclVolDenominator;
    mExclVolPressure    = model.mExclVolPressure;
    mQStats             = model.mQStats;
    mPar                = new TTMParameterSetBSQ(*(model.mPar));
    mBaryon             = model.mBaryon;
    mBeauty             = model.mBeauty;
    mBminus             = model.mBminus;
    mBplus              = model.mBplus;
    mbminus             = model.mbminus;
    mbplus              = model.mbplus;
    mCharge             = model.mCharge;
    mCharm              = model.mCharm;
    mCminus             = model.mCminus;
    mCplus              = model.mCplus;
    mDensity            = model.mDensity;

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();
    QHashIterator<int, TTMDensObj*> i(model.mDensTable);
    while (i.hasNext())
        mDensTable[i.key()] = i.value();

    mDescriptor = model.mDescriptor + " Copy";
    mEnergy     = model.mEnergy;
    mEntropy    = model.mEntropy;
    mPressure   = model.mPressure;
    mQminus     = model.mQminus;
    mQplus      = model.mQplus;
    mSminus     = model.mSminus;
    mSplus      = model.mSplus;
    mStrange    = model.mStrange;
    mWidth      = model.mWidth;
    mWroblewski = model.mWroblewski;

    setParent(model.parent());
}

//__________________________________________________________________________
TTMThermalModelBSQ::~TTMThermalModelBSQ()
{
    // dtor
    // mPar does not belong to this
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainBSQ(double baryon, double strange, double charge)
{
    // Code to find muB, muS, and muQ given B, S, Q and V (muC and mub not
    // constrained)
    //
    //
    int check;

    if (!mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))
        check = BSQConstrainBSQ(this, baryon, strange, charge);
    else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Constraint not yet coded");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainEoverN(double eoverN)
{
    // Constrains muB by the given E/N in the case where either both muS and
    // muQ or just muS are also to be constrained while muC and mub are not to be
    // constained. The other possibilities
    // are yet to be coded. 1 is returned if there are problems 0 otherwise.
    //

    int check;

    if (mPar->getConstrain(TTMParameterSet::kMUS) &&
            !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY)) {
        if (mPar->getConstrain(TTMParameterSet::kMUQ))
            check = BSQConstrainSQEN(this,eoverN);
        else
            check = BSQConstrainSEN(this,eoverN);
    } else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Constraint not yet coded");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainPercolation()
{
    // Constrains muB by the percolation model. 1 is returned if there are problems
    // 0 otherwise.
    //

    int check;

    if (mPar->getConstrain(TTMParameterSet::kMUQ) && mPar->getConstrain(TTMParameterSet::kMUS) &&
            !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainSQPercolation(this);

    else if (mPar->getConstrain(TTMParameterSet::kMUS) &&
             !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainSPercolation(this);

    else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Constraint not yet coded");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainQ(double charge)
{
    // Code to find muQ given Q and V (muS, muC and mub not
    // constrained)
    //
    //
    int check;

    if(!mPar->getConstrain(TTMParameterSet::kMUS)
            && !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))
        check = BSQConstrainQQ(this, charge);
    else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Constraint not yet coded");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainSoverT3(double soverT3)
{
    // Constrains muB by the given S/T^3 in the case where either both muS and
    // muQ or just muS are also to be constrained while muC and mub are not. The
    // other possibilities are yet to be coded. 1 is returned if there are
    // problems 0 otherwise.
    //

    int check;

    if(mPar->getConstrain(TTMParameterSet::kMUS) &&
            !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY)) {

        if(mPar->getConstrain(TTMParameterSet::kMUQ))
            check = BSQConstrainSQST3(this, soverT3);
        else
            check = BSQConstrainSST3(this, soverT3);
    } else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Constraint not yet coded");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
void TTMThermalModelBSQ::generateEnergyDens()
{
    // Iterates through the density hash table calculating the primordial
    // energy density of each particle in the hash table. Must first run
    // GenerateParticleDens() to populate the hash table. If the parameters
    // change then GenerateParticleDens() should be run again before this
    // function to impose constraints and ensure consistency between particle
    // and energy density.
    //


    QHashIterator<int, TTMDensObj*> next(mDensTable);

    mEnergy = 0.;
    double partEnergy;

    while (next.hasNext()) {
        TTMDensObj* dens = next.value();
        int part         = next.key();
        double width =  ParticlesDBManager::Instance().getWidth(part);
        int    stat  =  ParticlesDBManager::Instance().getStat(part);
        QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(part, mPar));

        if (!mExclVolCorrection) {
            if (stat == 0 || !mQStats) {		                 // Boltzmann stats
                if(width == 0 || !mWidth) 		                 // no width
                    partEnergy = ptr->energyBoltzmannNoWidth();
                else					                         // width
                    partEnergy = ptr->energyBoltzmannWidth();
            } else {					                        // Quantum stats
                if (width == 0 || !mWidth)	          	        // no width
                    partEnergy = ptr->energyQStatNoWidth();
                else				    	                    // width
                    partEnergy = ptr->energyQStatWidth();
            }

        } else {
            if (stat == 0 || !mQStats) {		                                  // Boltzmann stats
                if (width == 0 || !mWidth)   		                              // no width
                    partEnergy = ptr->energyBoltzmannNoWidth(mExclVolPressure);
                else		                         			                  // width
                    partEnergy = ptr->energyBoltzmannWidth(mExclVolPressure);
            } else {					                                          // Quantum stats
                if (width == 0 || !mWidth)		                                  // no width
                    partEnergy = ptr->energyQStatNoWidth(mExclVolPressure);
                else				             	                              // width
                    partEnergy = ptr->energyQStatWidth(mExclVolPressure);
            }
            partEnergy = partEnergy / mExclVolDenominator;
        }

        dens->setPrimaryEnergy(partEnergy);
        mEnergy += partEnergy;
    }
}

//__________________________________________________________________________
void TTMThermalModelBSQ::generateEntropyDens()
{
    // Iterates through the density hash table calculating the primordial
    // entropy density of each particle in the hash table. Must first run
    // GenerateParticleDens() to populate the hash table. If the parameters
    // change then GenerateParticleDens() should be run again before this
    // function to impose constraints and ensure consistency between particle
    // and entropy density.
    //

    mEntropy = 0.;
    double partEntropy;

    QHashIterator<int, TTMDensObj*> nextDens(mDensTable);
    while (nextDens.hasNext()) {
        TTMDensObj* dens = nextDens.value();
        int part         = nextDens.key();
        double width     = ParticlesDBManager::Instance().getWidth(part);
        int    stat      = ParticlesDBManager::Instance().getStat(part);
        QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(part, mPar));

        if (!mExclVolCorrection) {
            if (stat == 0 || !mQStats) {		       // Boltzmann stats
                if (width == 0 || !mWidth)		   // no width
                    partEntropy = ptr->entropyBoltzmannNoWidth();
                else				       	           // width
                    partEntropy = ptr->entropyBoltzmannWidth();
            } else {			   		              // Quantum stats
                if (width == 0 || !mWidth)		  // no width
                    partEntropy = ptr->entropyQStatNoWidth();
                else 				    	       // width
                    partEntropy = ptr->entropyQStatWidth();
            }
        } else {
            if (stat == 0 || !mQStats) {		       // Boltzmann stats
                if (width == 0 || !mWidth)		   // no width
                    partEntropy = ptr->entropyBoltzmannNoWidth(mExclVolPressure);
                else					               // width
                    partEntropy = ptr->entropyBoltzmannWidth(mExclVolPressure);
            } else {					               // Quantum stats
                if (width == 0 || !mWidth)		   // no width
                    partEntropy = ptr->entropyQStatNoWidth(mExclVolPressure);
                else				    	       // width
                    partEntropy = ptr->entropyQStatWidth(mExclVolPressure);
            }
            partEntropy = partEntropy / mExclVolDenominator;
        }
        dens->setPrimaryEntropy(partEntropy);
        mEntropy += partEntropy;
    }
}

//__________________________________________________________________________
int TTMThermalModelBSQ::generateParticleDens()
{
    // Calculates the Primordial particle densities and populates the density
    // hash table after first constraining muS and/or muQ and/or muC and/or
    // mub if required. The Wroblewski factor and the decay contributions are
    // also calculated provided the decays have been entered into the particle
    // set through TTMParticleSet::InputDecays(). Note: the Wroblewski factor
    // is only correct in the absence of charm and beauty...
    //

    int check = 1;

    if (mPar->getConstrain(TTMParameterSet::kMUS) && mPar->getConstrain(TTMParameterSet::kMUQ) &&
            !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainSQ(this);

    else if (mPar->getConstrain(TTMParameterSet::kMUS) &&
             !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))
        check = BSQConstrainS(this);

    else if (mPar->getConstrain(TTMParameterSet::kMUQ) &&
             !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainQ(this);

    else if (mPar->getConstrain(TTMParameterSet::kMUS) && mPar->getConstrain(TTMParameterSet::kMUQ) && mPar->getConstrain(TTMParameterSet::kMUC) &&
             !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainSQC(this);

    else if (mPar->getConstrain(TTMParameterSet::kMUS) && mPar->getConstrain(TTMParameterSet::kMUQ) &&
             mPar->getConstrain(TTMParameterSet::kMUC) && mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainSQCb(this);

    else if (!mPar->getConstrain(TTMParameterSet::kMUS) && !mPar->getConstrain(TTMParameterSet::kMUQ) &&
             !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = primPartDens();
    else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Constraint not yet coded");
        msg.exec();
    }
    if (!check) {
        calcWroblewski();
        generateDecayPartDens();
    } else {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Encountered some kind of problem");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
void TTMThermalModelBSQ::generatePressure()
{
    // Iterates through the density hash table calculating the primordial
    // pressure of each particle in the hash table. Must first run
    // GenerateParticleDens() to populate the hash table. If the parameters
    // change then GenerateParticleDens() should be run again before this
    // function to impose constraints and ensure consistency between particle
    // density and pressure.
    //

    mPressure = 0.;
    double partPressure;

    QHashIterator<int, TTMDensObj*> nextDens(mDensTable);
    while (nextDens.hasNext()) {
        TTMDensObj* dens = nextDens.value();
        int part         = nextDens.key();
        double width     = ParticlesDBManager::Instance().getWidth(part);
        int    stat      = ParticlesDBManager::Instance().getStat(part);
        QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(part, mPar));

        if (!mExclVolCorrection) {
            if (stat == 0 || !mQStats) {		                  // Boltzmann stats
                if (width == 0 || !mWidth)		                  // no width
                    partPressure = ptr->pressureBoltzmannNoWidth();
                else					                          // width
                    partPressure = ptr->pressureBoltzmannWidth();
            } else {					                        // Quantum stats
                if (width == 0 || !mWidth)		                // no width
                    partPressure = ptr->pressureQStatNoWidth();
                else				    	                    // width
                    partPressure = ptr->pressureQStatWidth();
            }
        } else {
            if (stat == 0 || !mQStats) {	                    // Boltzmann stats
                if (width == 0 || !mWidth)                      // no width
                    partPressure = ptr->pressureBoltzmannNoWidth(mExclVolPressure);
                else					                            // width
                    partPressure = ptr->pressureBoltzmannWidth(mExclVolPressure);
            } else {					                           // Quantum stats
                if (width == 0 || !mWidth)		                   // no width
                    partPressure = ptr->pressureQStatNoWidth(mExclVolPressure);
                else				    	                      // width
                    partPressure = ptr->pressureQStatWidth(mExclVolPressure);
            }
        }
        dens->setPrimaryPressure(partPressure);
        mPressure += partPressure;
    }
}

//__________________________________________________________________________
void TTMThermalModelBSQ::listInfo() const
{
  // Lists model information
  //
    QMessageBox msg(QMessageBox::Information, Q_FUNC_INFO, Q_FUNC_INFO);
    msg.setInformativeText("To be implemented");
    msg.exec();
  qDebug() <<"  ***************************************************************"
       <<"**************"
       ;
  qDebug() <<"  ***************************** Thermal Model Info **************"
       <<"**************"
        ;

  if(mQStats){
    qDebug() << "\t Quantum Statistics " ;
  }else{
    qDebug() << "\t Boltzmann Statistics " ;
  }
  if(mWidth){
    qDebug() << "\t Resonance width included " ;
  }else{
    qDebug() << "\t Resonance width excluded " ;
  }
  if(mExclVolCorrection){
    qDebug() << "\t Excluded volume corrections included " ;
  }else{
    qDebug() << "\t Excluded volume corrections excluded " ;
  }
  qDebug()  ;

  mPar->list();
  qDebug() <<"  ***************************** Thermal Quantities **************"
       <<"*************** "
        ;

  qDebug() << "\t S/V        = ";
  qDebug() << mStrange;
  qDebug() << "\t";
  if (mPar->getConstrain(TTMParameterSet::kMUS)) {
    qDebug() << "(constraint :";
    qDebug() << mPar->getDens(TTMParameterSet::kMUS) << ")";
  }
  qDebug()  ;
  qDebug() << "\t B/2Q     = ";
  qDebug() << mBaryon / 2. / mCharge;
  qDebug() << "\t";
  if (mPar->getConstrain(TTMParameterSet::kMUQ)) {
    qDebug() << "(constraint :";
    qDebug() << mPar->getB2Q() << ")";
  }
  qDebug()  ;
  qDebug() << "\t C/V        = ";
  qDebug() << mCharm;
  qDebug() << "\t";
  if (mPar->getConstrain(TTMParameterSet::kMUC)) {
    qDebug() << "(constraint :";
    qDebug() << mPar->getDens(TTMParameterSet::kMUC) << ")";
  }
  qDebug()  ;
  qDebug() << "\t b/V        = ";
  qDebug() << mBeauty;
  qDebug() << "\t";
  if (mPar->getConstrain(TTMParameterSet::kMUBEAUTY)) {
    qDebug() << "(constraint :";
    qDebug() << mPar->getDens(TTMParameterSet::kMUBEAUTY) << ")";
  }
  qDebug()  ;
  qDebug() << "\t lambda_s = ";
  qDebug() << mWroblewski ;
  qDebug()  ;
  qDebug() << "\t Primordial Densities: " ;
  if (mDensity != 0.) {
    qDebug() << "\t\t\t\t n = " << mDensity ;
  }
  if (mEnergy != 0.) {
    qDebug() << "\t\t\t\t e = " << mEnergy ;
  }
  if (mEntropy != 0.) {
    qDebug() << "\t\t\t\t s = " << mEntropy  ;
  }
  if (mPressure != 0.) {
    qDebug() << "\t\t\t\t P = " << mPressure  ;
  }
  QHashIterator<int, TTMParticle*> part(ParticlesDBManager::Instance().allParticles());
  while (part.hasNext()) {
      part.next();
      TTMParticle* particle = part.value();
      if (!particle->isStable())
          continue;
      qDebug() << Q_FUNC_INFO << particle->getName();
  }
  qDebug()  ;
  qDebug() <<"  *************************************************************"
       <<"*****************"
       ;
  qDebug() <<"  *************************************************************"
       <<"*****************"
        ;

}

//__________________________________________________________________________
int TTMThermalModelBSQ::primPartDens()
{
    // Calculates the primordial particle densities and populates the density
    // hash table with these values. The parameters are not constrained first!
    // This is the function used by GenerateParticleDens().
    //

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();


    mSplus    = mSminus = mBplus = mBminus = mQplus = mQminus = mCplus = mCminus = mbplus = mbminus = 0.;
    mStrange  = mBaryon = mCharge = mDensity = mWroblewski = mCharm = mBeauty = 0.;
    mEnergy   = mEntropy = mPressure = 0.;

    mExclVolPressure    = 0.;
    mExclVolDenominator = 1.;

    int check = 0;

    if (mQStats) {
        QHashIterator<int, TTMParticle*> part(ParticlesDBManager::Instance().allParticles());
        while (part.hasNext()) {
            part.next();
            int pdg = part.key();
            if (check != 0)
                break;
            QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(pdg, mPar));
            if(!ptr->parametersAllowed())
                check = 1;
        }
    }
    if (check) {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Chemical Potentials not allowed!");
        msg.exec();
        return 1;
    } else {
        if (!mExclVolCorrection) {
                QHashIterator<int, TTMParticle*> particles(ParticlesDBManager::Instance().allParticles());
                while (particles.hasNext()) {
                    particles.next();
                    int pdg               = particles.key();
                    TTMParticle* particle = particles.value();
                QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(pdg, mPar));
                double partDens;
                double width = particle->getWidth();
                int    stat  = particle->getStat();
                if (stat == 0 || !mQStats) {		                    // Boltzmann stats
                    if (width == 0 || !mWidth) 		                    // no width
                        partDens = ptr->densityBoltzmannNoWidth();
                    else 					                           // width
                        partDens = ptr->densityBoltzmannWidth();
                } else {					                           // Quantum stats
                    if (width == 0 || !mWidth)           		       // no width
                        partDens = ptr->densityQStatNoWidth();
                    else  				    	                       // width
                        partDens = ptr->densityQStatWidth();
                }
                TTMDensObj* dens = new TTMDensObj(pdg);
                dens->setPrimaryDensity(partDens);
                mDensTable[pdg] = dens;
                mDensity += partDens;

                double strange = particle->getS();
                if (strange > 0.)
                    mSplus  += partDens * strange;
                else if (strange < 0.)
                    mSminus += partDens * strange;

                double baryon = particle->getBaryon();
                if (baryon > 0.)
                    mBplus  += partDens * baryon;
                else if (baryon < 0.)
                    mBminus += partDens * baryon;

                double charge = particle->getCharge();
                if (charge > 0.)
                    mQplus  += partDens * charge;
                else if (charge < 0.)
                    mQminus += partDens * charge;

                double charm = particle->getCharm();
                if (charm > 0.)
                    mCplus  += partDens * charm;
                else if (charm < 0.)
                    mCminus += partDens * charm;

                double beauty = particle->getBeauty();
                if (beauty > 0.)
                    mbplus  += partDens * beauty;
                else if (beauty < 0.)
                    mbminus += partDens * beauty;
            }
        } else {
            calcExclVolPressure();
            double correction = 1.;
            QHashIterator<int, TTMParticle*> particles(ParticlesDBManager::Instance().allParticles());
            while (particles.hasNext()) {
                particles.next();
                int pdg               = particles.key();
                TTMParticle* particle = particles.value();
                QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(pdg, mPar));
                double partDens;
                double width = particle->getWidth();
                int    stat  = particle->getStat();
                if (stat == 0 || !mQStats) {		                          // Boltzmann stats
                    if (width == 0 || !mWidth)                   	          // no width
                        partDens = ptr->densityBoltzmannNoWidth(mExclVolPressure);
                    else					                                  // width
                        partDens = ptr->densityBoltzmannWidth(mExclVolPressure);

                } else {        					                         // Quantum stats
                    if (width == 0 || !mWidth)	 	                         // no width
                        partDens = ptr->densityQStatNoWidth(mExclVolPressure);
                    else				     	                             // width
                        partDens = ptr->densityQStatWidth(mExclVolPressure);
                }

                TTMDensObj *dens = new TTMDensObj(pdg);
                dens->setPrimaryDensity(partDens);	// ideal gas with shifted chemical potential

                mDensTable[pdg] = dens;

                double radius = particle->getRadius();
                double volume = 4. * 4./3. * M_PI * qPow(radius, 3);
                correction += volume * partDens;
            }
            mExclVolDenominator = correction;
            QHashIterator<int, TTMDensObj*> nextDens(mDensTable);
            while (nextDens.hasNext()) {
                TTMDensObj* dens = nextDens.value();
                int particle     = nextDens.key();
                dens->setPrimaryDensity(dens->getPrimaryDensity() / mExclVolDenominator);
                double partDens = dens->getPrimaryDensity();
                mDensity += partDens;

                double strange = ParticlesDBManager::Instance().getS(particle);
                if (strange > 0.)
                    mSplus  += partDens * strange;
                else if (strange < 0.)
                    mSminus += partDens * strange;

                double baryon = ParticlesDBManager::Instance().getBaryon(particle);
                if (baryon > 0.)
                    mBplus  += partDens * baryon;
                else if (baryon < 0.)
                    mBminus += partDens * baryon;

                double charge= ParticlesDBManager::Instance().getCharge(particle);
                if (charge > 0.)
                    mQplus  += partDens * charge;
                else if (charge < 0.)
                    mQminus += partDens * charge;

                double charm = ParticlesDBManager::Instance().getCharm(particle);
                if (charm > 0.)
                    mCplus  += partDens * charm;
                else if (charm < 0.)
                    mCminus += partDens * charm;

                double beauty = ParticlesDBManager::Instance().getBeauty(particle);
                if (beauty > 0.)
                    mbplus  += partDens * beauty;
                else if (beauty < 0.)
                    mbminus += partDens * beauty;
            }
        }

        mStrange = mSplus + mSminus;
        mBaryon  = mBplus + mBminus;
        mCharge  = mQplus + mQminus;
        mCharm   = mCplus + mCminus;
        mBeauty  = mbplus + mbminus;

        return 0;
    }
}

//__________________________________________________________________________
void TTMThermalModelBSQ::reset()
{
    // reset everything except mPartPDGs;
    // from the base class

    mBaryon = mBeauty = mBminus = mBplus = mbminus = mbplus = mCharge = mCharm = mCminus = mCplus = mDensity = 0;

    mEnergy = mEntropy = mPressure = mQminus = mQplus = mSminus = mSplus = mStrange = mWidth = mWroblewski = 0;

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();

    // from this class
    mWidth              = true;
    mPar                = nullptr;
    mExclVolCorrection  = false;
    mExclVolDenominator = 1.0;
    mExclVolPressure    = 0.0;
    mQStats             = true;

}

//__________________________________________________________________________
TTMThermalModelBSQ& TTMThermalModelBSQ::operator=(TTMThermalModelBSQ model)
{
    // assignation operator
    TTMThermalModelBSQ tmp(model);
    mExclVolCorrection  = model.mExclVolCorrection;
    mExclVolDenominator = model.mExclVolDenominator;
    mExclVolPressure    = model.mExclVolPressure;
    mPar                = new TTMParameterSetBSQ(*(model.mPar));
    mQStats             = model.mQStats;

    return *this;
}



//__________________________________________________________________________
void TTMThermalModelBSQ::calcExclVolPressure()
{
    // Calculates the total pressure in the system with excluded volume
    // corrections
    //

    TTMParameterSetBSQ copyPar = *mPar;

    TTMThermalModelBSQ copyMod(&copyPar, mQStats, mWidth);

    copyMod.setExcludedVolume(false);
    copyMod.primPartDens();
    copyMod.generatePressure();

    double pressure = copyMod.getPressure();

    mExclVolPressure = FindExclVolPressure(this, pressure);
}

//__________________________________________________________________________
double TTMThermalModelBSQ::exclVolShiftedPressure(double px)
{
    // Calculates the total ideal gas pressure corresponding to chemical
    // potentials shifted by an amount px
    //

    double totalShiftedPressure = 0.;
    QHashIterator<int, TTMParticle*> part(ParticlesDBManager::Instance().allParticles());
    while (part.hasNext()) {
        part.next();
        int pdg = part.key();
        QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(pdg, mPar));
        double partPressure;
        double width = ParticlesDBManager::Instance().getWidth(pdg);
        int    stat  = ParticlesDBManager::Instance().getStat(pdg);

        if (stat == 0 || !mQStats) {           		       // Boltzmann stats
            if (width == 0 || !mWidth) 		               // no width
                partPressure = ptr->pressureBoltzmannNoWidth(px);
            else					                           // width
                partPressure = ptr->pressureBoltzmannWidth(px);
        } else {					                      // Quantum stats
            if (width == 0 || !mWidth)		              // no width
                partPressure = ptr->pressureQStatNoWidth(px);
            else				    	                      // width
                partPressure = ptr->pressureQStatWidth(px);
        }

        totalShiftedPressure += partPressure;
    }
    return totalShiftedPressure;
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainTotalBaryonDensity(double nb)
{
    // Constrains muB by the given nb in the case where either just muS or else
    // both muS and muQ are also to be constrained while muC and mub are not
    // constrained. The other possibilities are
    // yet to be coded. 1 is returned if there are problems 0 otherwise.
    //

    int check;

    if (mPar->getConstrain(TTMParameterSet::kMUS) &&
            !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY)) {

        if (mPar->getConstrain(TTMParameterSet::kMUQ))
            check = BSQConstrainSQBDens(this, nb);

        else
            check = BSQConstrainSBDens(this, nb);
    } else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Such constraint not yet coded");
        msg.exec();
    }
    return check;
}

//__________________________________________________________________________
int TTMThermalModelBSQ::constrainNetBaryonDensity(double nb)
{
    // Constrains muB by the given nb in the case where
    // both muS and muQ are also to be constrained while muC and mub are not
    // to be constrained. The other possibilities are
    // yet to be coded. 1 is returned if there are problems 0 otherwise.
    //

    int check;

    if (mPar->getConstrain(TTMParameterSet::kMUS) && mPar->getConstrain(TTMParameterSet::kMUQ) &&
            !mPar->getConstrain(TTMParameterSet::kMUC) && !mPar->getConstrain(TTMParameterSet::kMUBEAUTY))

        check = BSQConstrainSQNetBDens(this,nb);

    else {
        check = 1;
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Such constraint not yet coded");
        msg.exec();
    }
    return check;
}
