// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// Strangeness Canonical thermal model class.


#include <gsl/gsl_sf_bessel.h>

#include <QMessageBox>
#include <QtMath>
#include <QScopedPointer>

#include "external/particlesdbmanager.h"
#include "functions/FncsConstrain.h"

#include "TTMDensObj.h"
#include "TTMParameterSetBQ.h"
#include "TTMParameterSetBSQ.h"
#include "TTMThermalModelBQ.h"
#include "TTMThermalParticleBQ.h"
#include "TTMThermalParticleBSQ.h"

const double kLN10 = qLn(10.0);
//__________________________________________________________________________
TTMThermalModelBQ::TTMThermalModelBQ(QObject *parent) : TTMThermalModel(parent),
    mNonStrangeQStats(true), mPar(nullptr)
{
    //ctor
    mDescriptor       = "SCanonical";
    mWidth            = true;

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();

    mSplus   = mSminus = mBplus = mBminus = mQplus = mQminus = mCplus = mCminus = mbplus = mbminus = 0.;
    mStrange = mBaryon = mCharge = mDensity = mWroblewski = mCharm = mBeauty = 0.;
    mEnergy  = mEntropy = 0.;
}

//__________________________________________________________________________
TTMThermalModelBQ::TTMThermalModelBQ(TTMParameterSetBQ *parameters, bool width, QObject *parent) : TTMThermalModel(parent),
    mNonStrangeQStats(true), mPar(parameters)
{
    mDescriptor = "SCanonical";
    mPar   = parameters;
    mWidth = width;

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();

    mSplus   = mSminus = mBplus = mBminus = mQplus = mQminus = mCplus = mCminus = mbplus = mbminus = 0.;
    mStrange = mBaryon = mCharge = mDensity = mWroblewski = mCharm = mBeauty = 0.;
    mEnergy  = mEntropy = 0.;
}

int TTMThermalModelBQ::constrainEoverN(Double_t eovern)
{
    // Constrains muB by the given E/N. 1 is returned if there are problems
    // 0 otherwise.
    //

    Int_t check;

    if(!fParm->GetMuQConstrain()){

      check = BQConstrainEN(this,EoverN);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }else{

      check = BQConstrainQEN(this,EoverN);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }

    return check;
}

int TTMThermalModelBQ::constrainNetBaryonDensity(Double_t nb)
{
    // Constrains muB by the given nb. 1 is returned if there are problems
    // 0 otherwise.
    //

    Int_t check;

    if(fParm->GetMuQConstrain()){

      check = BQConstrainQNetBDens(this,nb);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }else{

      check = 1;
      cout<<"Constraint not yet coded"<<endl;

    }

    return check;

}

int TTMThermalModelBQ::constrainPercolation()
{

    // Constrains muB by the percolation model. 1 is returned if there are problems
    // 0 otherwise.
    //

    Int_t check;

    if(!fParm->GetMuQConstrain()){

        check = 1;
        cout<<"Such Constraint Not Yet Coded"<<endl;

    }else{

      check = BQConstrainQPercolation(this);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }

    return check;

}

int TTMThermalModelBQ::constrainSoverT3(Double_t SoverT3)
{
    // Constrains muB by the given S/T^3. 1 is returned if there are problems
    // 0 otherwise.
    //

    Int_t check;

    if(!fParm->GetMuQConstrain()){

      check = BQConstrainST3(this,SoverT3);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }else{

      check = BQConstrainQST3(this,SoverT3);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }

    return check;

}

int TTMThermalModelBQ::constrainTotalBaryonDensity(Double_t nb)
{
    // Constrains muB by the given nb. 1 is returned if there are problems
    // 0 otherwise.
    //

    Int_t check;

    if(!fParm->GetMuQConstrain()){

      check = BQConstrainBDens(this,nb);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }else{

      check = BQConstrainQBDens(this,nb);
      if(check){
        cout<<"Not Constrained"<<endl;
      }

    }

    return check;

}

void TTMThermalModelBQ::generateEnergyDens()
{
    // Iterates through the density hash table calculating the primordial
     // energy density of each particle in the hash table. Must first run
     // GenerateParticleDens() to populate the hash table. If the parameters
     // change then GenerateParticleDens() should be run again before this
     // function to impose constraints and calculate the canonical correction
     // factors.
     //

     TIter next(fDensTable);
     TTMDensObj *dens;

     fEnergy = 0.;

     while ((dens = (TTMDensObj *) next())) {
       TTMParticle *part = fPartSet->GetParticle(dens->GetID());
       Double_t CorrFactor;

       if (part->GetS() == 1) {
         CorrFactor = fCorrP1;
       } else if (part->GetS() == 2) {
         CorrFactor = fCorrP2;
       } else if (part->GetS() == 3) {
         CorrFactor = fCorrP3;
       } else if (part->GetS() == -1) {
         CorrFactor = fCorrM1;
       } else if (part->GetS() == -2) {
         CorrFactor = fCorrM2;
       } else if (part->GetS() == -3) {
         CorrFactor = fCorrM3;
       } else {
         CorrFactor = 1.;
       }

       Double_t PartEnergy;

       if(part->GetS() == 0. && fNonStrangeQStats){

         TTMParameterSetBSQ pGC(fParm->GetT(),fParm->GetMuB(),0.,fParm->GetMuQ(),fParm->GetGammas());

         TTMThermalParticleBSQ *ptr = new TTMThermalParticleBSQ(part, &pGC);

         if(part->GetWidth() == 0 || !fWidth){
           PartEnergy = ptr->EnergyQStatNoWidth();
         }else{
           PartEnergy = ptr->EnergyQStatWidth();
         }

         delete ptr;

       }else{

         TTMThermalParticleBQ *ptr = new TTMThermalParticleBQ(part, fParm, CorrFactor);

         if(part->GetWidth() == 0 || !fWidth){
           PartEnergy = ptr->EnergyBoltzmannNoWidth();
         }else{
           PartEnergy = ptr->EnergyBoltzmannWidth();
         }

         delete ptr;

       }

       dens->SetPrimEnergy(PartEnergy);
       fEnergy += PartEnergy;
     }
}

void TTMThermalModelBQ::generateEntropyDens()
{
    // Iterates through the density hash table calculating the primordial
     // entropy density of each particle in the hash table. Must first run
     // GenerateParticleDens() to populate the hash table. If the parameters
     // change then GenerateParticleDens() should be run again before this
     // function to impose constraints and calculate the canonical correction
     // factors. Remember that the total entropy does not split into the sum
     // of particle entropies.
     //

     TIter next(fDensTable);
     TTMDensObj *dens;

     fEntropy = 0.;

     while ((dens = (TTMDensObj *) next())) {
       TTMParticle *part = fPartSet->GetParticle(dens->GetID());
       Double_t CorrFactor;

       if (part->GetS() == 1) {
         CorrFactor = fCorrP1;
       } else if (part->GetS() == 2) {
         CorrFactor = fCorrP2;
       } else if (part->GetS() == 3) {
         CorrFactor = fCorrP3;
       } else if (part->GetS() == -1) {
         CorrFactor = fCorrM1;
       } else if (part->GetS() == -2) {
         CorrFactor = fCorrM2;
       } else if (part->GetS() == -3) {
         CorrFactor = fCorrM3;
       } else {
         CorrFactor = 1.;
       }

       Double_t PartEntropy;

       if(part->GetS() == 0.){

         TTMParameterSetBSQ pGC(fParm->GetT(),fParm->GetMuB(),0.,fParm->GetMuQ(),fParm->GetGammas());

         TTMThermalParticleBSQ *ptr = new TTMThermalParticleBSQ(part, &pGC);

         if(fNonStrangeQStats){

           if(part->GetWidth() == 0 || !fWidth){
             PartEntropy = ptr->EntropyQStatNoWidth();
           }else{
             PartEntropy = ptr->EntropyQStatWidth();
           }

         }else{

           if(part->GetWidth() == 0 || !fWidth){
             PartEntropy = ptr->EntropyBoltzmannNoWidth();
           }else{
             PartEntropy = ptr->EntropyBoltzmannWidth();
           }

         }

         delete ptr;

       }else{

         TTMThermalParticleBQ *ptr = new TTMThermalParticleBQ(part, fParm, CorrFactor);

         Double_t PartEnergy;

         if(part->GetWidth() == 0 || !fWidth){
           PartEnergy = ptr->EnergyBoltzmannNoWidth();
         }else{
           PartEnergy = ptr->EnergyBoltzmannWidth();
         }

         delete ptr;

         Double_t PartDens = dens->GetPrimDensity();

         Double_t B = part->GetB();
         Double_t Q = part->GetQ();
         Double_t muB = fParm->GetMuB();
         Double_t muQ = fParm->GetMuQ();

         Double_t mu = B*muB + Q*muQ;

         PartEntropy = (PartEnergy - mu * PartDens) / fParm->GetT() ;

       }

       dens->SetPrimEntropy(PartEntropy);
       fEntropy += PartEntropy;

     }

     Double_t r = fParm->GetCanRadius();
     Double_t volume = 4. * TMath::Pi() / 3. * r * r * r;

     fEntropy += (flnZtot - flnZ0)/volume;

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
    if (mPar->getMuQConstrain()) {
        if (BQConstrainQ(this))
            check = 1;
        else
            check = 0;
    } else
        check = primPartDens();
    if (!check) {
        calcWroblewski();
        generateDecayPartDens();
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Problems");
        msg.exec();
    }
    return check;
}

void TTMThermalModelBQ::generatePressure()
{
    // Iterates through the density hash table calculating the primordial
    // pressure of each particle in the hash table. Must first run
    // GenerateParticleDens() to populate the hash table. If the parameters
    // change then GenerateParticleDens() should be run again before this
    // function to impose constraints and calculate the canonical correction
    // factors.
    //

    TIter next(fDensTable);
    TTMDensObj *dens;

    fPressure = 0.;

    while ((dens = (TTMDensObj *) next())) {
      TTMParticle *part = fPartSet->GetParticle(dens->GetID());
      Double_t CorrFactor;

      if (part->GetS() == 1) {
        CorrFactor = fCorrP1;
      } else if (part->GetS() == 2) {
        CorrFactor = fCorrP2;
      } else if (part->GetS() == 3) {
        CorrFactor = fCorrP3;
      } else if (part->GetS() == -1) {
        CorrFactor = fCorrM1;
      } else if (part->GetS() == -2) {
        CorrFactor = fCorrM2;
      } else if (part->GetS() == -3) {
        CorrFactor = fCorrM3;
      } else {
        CorrFactor = 1.;
      }

      Double_t PartPressure;

      if(part->GetS() == 0. && fNonStrangeQStats){

        TTMParameterSetBSQ pGC(fParm->GetT(),fParm->GetMuB(),0.,fParm->GetMuQ(),fParm->GetGammas());

        TTMThermalParticleBSQ *ptr = new TTMThermalParticleBSQ(part, &pGC);

        if(part->GetWidth() == 0 || !fWidth){
          PartPressure = ptr->PressureQStatNoWidth();
        }else{
          PartPressure = ptr->PressureQStatWidth();
        }

        delete ptr;

      }else{

        TTMThermalParticleBQ *ptr = new TTMThermalParticleBQ(part, fParm, CorrFactor);

        if(part->GetWidth() == 0 || !fWidth){
          PartPressure = ptr->PressureBoltzmannNoWidth();
        }else{
          PartPressure = ptr->PressureBoltzmannWidth();
        }

        delete ptr;

      }

      dens->SetPrimPressure(PartPressure);
      fPressure += PartPressure;
    }
}

void TTMThermalModelBQ::listInfo()
{
    // List model information
      //

      cout <<"  ************************************************************"
           <<"*****************"
           << endl;
      cout <<"  ***************************** Thermal Model Info ***********"
           <<"*****************"
           << endl << endl;

      cout << "\t Particle set: " << endl << "\t\t" << fPartSet->GetFilename()
           << endl << endl;

      if(fNonStrangeQStats){
        cout << "\t Quantum statistics for S=0 hadrons" << endl;
      }else{
        cout << "\t Boltzmann statistics for S=0 hadrons" << endl;
      }
      cout << "\t Boltzmann statistics for strange hadrons" << endl;

      if(fWidth){
        cout << "\t Resonance width included " << endl;
      }else{
        cout << "\t Resonance width excluded " << endl;
      }

      cout << endl << endl;

      fParm->List();
      cout <<"  ***************************** Thermal Quantities ***********"
           <<"****************** "
           << endl << endl;
      cout << "\t S required in canonical volume: ";
      cout.width(10);
      cout << fParm->GetS();
      cout << endl << endl;
      Double_t r = fParm->GetCanRadius();
      Double_t volume = 4. * TMath::Pi() / 3. * r * r * r;
      cout << "\t S in canonical volume (model) = ";
      cout.width(10);
      cout << fStrange*volume;
      cout << endl << endl;
      cout << "\t B/2Q     = ";
      cout.width(10);
      cout << fBaryon / 2. / fCharge;
      cout << "\t";
      if (fParm->GetMuQConstrain()) {
        cout << "(constraint :";
        cout.width(8);
        cout << fParm->GetB2Q() << ")";
      }
      cout << endl << endl;
      cout << "\t lambda_s = ";
      cout.width(8);
      cout << fWroblewski << endl;
      cout << endl << endl;
      cout << "\t Primordial Densities: " << endl;
      if (fDensity != 0.) {
        cout << "\t\t\t\t n = " << fDensity << endl;
      }
      if (fEnergy != 0.) {
        cout << "\t\t\t\t e = " << fEnergy << endl;
      }
      if (fEntropy != 0.) {
        cout << "\t\t\t\t s = " << fEntropy << endl << endl;
      }
      ListStableParticles();
      cout << endl << endl;
      cout <<"  ***********************************************************"
           <<"*******************"
           << endl;
      cout <<"  ***********************************************************"
           <<"*******************"
           << endl << endl;
}

//__________________________________________________________________________
int TTMThermalModelBQ::primPartDens()
{
    // Calculates the primordial particle densities and populates the density
    // hash table. The parameters are not constrained first!. This is the
    // function used by GenerateParticleDens() and calls term().
    // Returns 1 if there are problems 0 else.

    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();

    mSplus    = mSminus = mBplus = mBminus = mQplus = mQminus = mCplus = mCminus = mbplus = mbminus = 0.;
    mStrange  = mBaryon = mCharge = mDensity = mWroblewski = mCharm = mBeauty = 0.;
    mEnergy   = mEntropy = 0.;
    int check = 0;

    QList<int> partPDGs;

    if (mNonStrangeQStats) {
        ParticlesDBManager::Instance().allParticles(partPDGs);
        for (int part : partPDGs) {
            if ( check != 0)
                break;
            if(ParticlesDBManager::Instance().getS(part) == 0.) {
                TTMParameterSetBSQ pGC(mPar->getT(), mPar->getMuB(), 0., mPar->getMuQ(), mPar->getGammas());
                QScopedPointer<TTMThermalParticleBSQ>ptr(new TTMThermalParticleBSQ(part,&pGC));

                //                TTMThermalParticleBSQ *ptr = new TTMThermalParticleBSQ(part,&pGC);

                if(!ptr->parametersAllowed()){
                    check = 1;
                }
            }
        }
    }

    if (check) {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "Chemical Potentials not allowed!");
        msg.exec();
        return 1;
    } else {
        double z      = 0.;
        double k0     = 0., kp1 = 0., kp2 = 0., kp3 = 0., km1 = 0., km2 = 0., km3 = 0.;
        double r      = mPar->getCanRadius();
        double volume = 4. * M_PI / 3. * r * r * r;

        for (int part : partPDGs) {
            double width = ParticlesDBManager::Instance().getWidth(part);
            double s     = ParticlesDBManager::Instance().getS(part);
            if (s == 0. && mNonStrangeQStats){
                double gCPressure;
                TTMParameterSetBSQ pGC(mPar->getT(), mPar->getMuB(), 0., mPar->getMuQ(), mPar->getGammas());
                QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(part, &pGC));
                if (width == 0. || !mWidth)
                    gCPressure = ptr->pressureQStatNoWidth();
                else
                    gCPressure = ptr->pressureQStatWidth();
                z = gCPressure * volume / mPar->getT();
            } else {
                double gCPartDens;
                QScopedPointer<TTMThermalParticleBQ> ptr(new TTMThermalParticleBQ(part, mPar, 1));
                if (width == 0 || !mWidth)
                    gCPartDens = ptr->densityBoltzmannNoWidth();
                else
                    gCPartDens = ptr->densityBoltzmannWidth();
                z = gCPartDens * volume;    // only true in Boltzmann case since PV/T = nV
            }

            if (s == 0.)
                k0 += z;
            else if (s == 1.)
                kp1 += z;
            else if (s == 2.)
                kp2 += z;
            else if (s == 3.)
                kp3 += z;
            else if (s == -1.)
                km1 += z;
            else if (s == -2.)
                km2 += z;
            else if (s == -3.)
                km3 += z;
        }

        double x[3];

        x[0] = 2. * qSqrt(kp1 * km1);
        x[1] = 2. * qSqrt(kp2 * km2);
        x[2] = 2. * qSqrt(kp3 * km3);

        double y[3];

        y[0] = qSqrt(kp1 / km1);
        y[1] = qSqrt(kp2 / km2);
        y[2] = qSqrt(kp3 / km3);

        double xmax = 709.;		// maximum argument for Bessel fncs in ROOT

        if (x[0] > xmax || x[1] > xmax || x[2] > xmax ) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "x's too large");
            msg.exec();
            return 1;
        } else if (y[0] == 0 || y[1] == 0 || y[2] == 0) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "y's zero");
            msg.exec();
            return 1;
        } else {
            double TSPartition3 = 0.0;
            double Tfactorkm1   = 0.0;
            double Tfactorkp1   = 0.0;
            double Tfactorkm2   = 0.0;
            double Tfactorkp2   = 0.0;
            double Tfactorkm3   = 0.0;
            double Tfactorkp3   = 0.0;
            double rterm[7];
            // Include (0,0) term //

            term(x, y, 0, 0, rterm);

            TSPartition3 = rterm[0];
            Tfactorkm1   = rterm[4];
            Tfactorkp1   = rterm[1];
            Tfactorkm2   = rterm[5];
            Tfactorkp2   = rterm[2];
            Tfactorkm3   = rterm[6];
            Tfactorkp3   = rterm[3];

            double dZ;
            double dp1;
            double dp2;
            double dp3;
            double dm1;
            double dm2;
            double dm3;
            // First Quandrant +m and +n axes included //
            int    m = 1, n = 0;
            int    mmax;
            double tol = 1e-40;

            do {
                do {
                    term(x, y, m, n, rterm);

                    TSPartition3 += rterm[0];
                    Tfactorkm1   += rterm[4];
                    Tfactorkp1   += rterm[1];
                    Tfactorkm2   += rterm[5];
                    Tfactorkp2   += rterm[2];
                    Tfactorkm3   += rterm[6];
                    Tfactorkp3   += rterm[3];

                    dZ  = rterm[0] / TSPartition3;
                    dp1 = rterm[1] / Tfactorkp1;
                    dp2 = rterm[2] / Tfactorkp2;
                    dp3 = rterm[3] / Tfactorkp3;
                    dm1 = rterm[4] / Tfactorkm1;
                    dm2 = rterm[5] / Tfactorkm2;
                    dm3 = rterm[6] / Tfactorkm3;

                    m++;
                } while (dZ > tol);
                n++;
                mmax = m - 1;
                m    = 0;
            } while (mmax > 0);

            // Second Quandrant -m axis included

            n = 0;
            m = -1;

            do{
                do{
                    term(x, y, m, n, rterm);

                    TSPartition3 += rterm[0];
                    Tfactorkm1   += rterm[4];
                    Tfactorkp1   += rterm[1];
                    Tfactorkm2   += rterm[5];
                    Tfactorkp2   += rterm[2];
                    Tfactorkm3   += rterm[6];
                    Tfactorkp3   += rterm[3];

                    dZ  = rterm[0] / TSPartition3;
                    dp1 = rterm[1] / Tfactorkp1;
                    dp2 = rterm[2] / Tfactorkp2;
                    dp3 = rterm[3] / Tfactorkp3;
                    dm1 = rterm[4] / Tfactorkm1;
                    dm2 = rterm[5] / Tfactorkm2;
                    dm3 = rterm[6] / Tfactorkm3;

                    m--;
                } while (dZ > tol);
                n++;
                mmax = m + 1;
                m = -1;
            } while (mmax < -1);

            // Third Quandrant -n axis included

            n = -1;
            m = 0;

            do{
                do{
                    term(x, y, m, n, rterm);

                    TSPartition3 += rterm[0];
                    Tfactorkm1   += rterm[4];
                    Tfactorkp1   += rterm[1];
                    Tfactorkm2   += rterm[5];
                    Tfactorkp2   += rterm[2];
                    Tfactorkm3   += rterm[6];
                    Tfactorkp3   += rterm[3];

                    dZ  = rterm[0] / TSPartition3;
                    dp1 = rterm[1] / Tfactorkp1;
                    dp2 = rterm[2] / Tfactorkp2;
                    dp3 = rterm[3] / Tfactorkp3;
                    dm1 = rterm[4] / Tfactorkm1;
                    dm2 = rterm[5] / Tfactorkm2;
                    dm3 = rterm[6] / Tfactorkm3;

                    m--;
                } while ( dZ > tol);
                n--;
                mmax = m + 1;
                m = 0;
            } while (mmax < 0);

            // Fourth Quandrant

            n = -1;
            m = 1;

            do{
                do{
                    term(x, y, m, n, rterm);

                    TSPartition3 += rterm[0];
                    Tfactorkm1   += rterm[4];
                    Tfactorkp1   += rterm[1];
                    Tfactorkm2   += rterm[5];
                    Tfactorkp2   += rterm[2];
                    Tfactorkm3   += rterm[6];
                    Tfactorkp3   += rterm[3];

                    dZ  = rterm[0] / TSPartition3;
                    dp1 = rterm[1] / Tfactorkp1;
                    dp2 = rterm[2] / Tfactorkp2;
                    dp3 = rterm[3] / Tfactorkp3;
                    dm1 = rterm[4] / Tfactorkm1;
                    dm2 = rterm[5] / Tfactorkm2;
                    dm3 = rterm[6] / Tfactorkm3;

                    m++;
                } while (dZ > tol);
                n--;
                mmax = m - 1;
                m = 1;
            } while (mmax > 1);

            mCorrP1 = Tfactorkp1 / TSPartition3;
            mCorrP2 = Tfactorkp2 / TSPartition3;
            mCorrP3 = Tfactorkp3 / TSPartition3;
            mCorrM1 = Tfactorkm1 / TSPartition3;
            mCorrM2 = Tfactorkm2 / TSPartition3;
            mCorrM3 = Tfactorkm3 / TSPartition3;

            mlnZtot   = qLn(TSPartition3) + qLn(gsl_sf_bessel_In(0, x[0])) + qLn(gsl_sf_bessel_In(0, x[1])) + qLn(gsl_sf_bessel_In(0, x[2])) + k0;
            mlnZ0     = k0;
            mExactMuS = mPar->getT() * qLn(mCorrP1);

            for (int part : partPDGs) {
                double S = ParticlesDBManager::Instance().getS(part);
                double CorrFactor;
                if (S == 1)
                    CorrFactor = mCorrP1;
                else if (S == 2)
                    CorrFactor = mCorrP2;
                else if (S == 3)
                    CorrFactor = mCorrP3;
                else if (S == -1)
                    CorrFactor = mCorrM1;
                else if (S == -2)
                    CorrFactor = mCorrM2;
                else if (S == -3)
                    CorrFactor = mCorrM3;
                else
                    CorrFactor = 1.;

                double PartDens;

                double width = ParticlesDBManager::Instance().getWidth(part);
                if (S == 0. && mNonStrangeQStats) {
                    TTMParameterSetBSQ pGC(mPar->getT(), mPar->getMuB(), 0., mPar->getMuQ(), mPar->getGammas());
                    QScopedPointer<TTMThermalParticleBSQ> ptr(new TTMThermalParticleBSQ(part, &pGC));

                    if( width == 0.0 || !mWidth)
                        PartDens = ptr->densityQStatNoWidth();
                    else
                        PartDens = ptr->densityQStatWidth();
                } else {
                    QScopedPointer<TTMThermalParticleBQ> ptr(new TTMThermalParticleBQ(part, mPar, CorrFactor));
                    if (width == 0.0 || !mWidth)
                        PartDens = ptr->densityBoltzmannNoWidth();
                    else
                        PartDens = ptr->densityBoltzmannWidth();
                }
                TTMDensObj *dens = new TTMDensObj(part);
                dens->setPrimaryDensity(PartDens);
                mDensTable[part] = dens;

                mDensity += PartDens;

                double strange = ParticlesDBManager::Instance().getS(part);
                if (strange > 0.)
                    mSplus += PartDens * strange;
                else if (strange < 0.)
                    mSminus += PartDens * strange;

                double baryon = ParticlesDBManager::Instance().getBaryon(part);
                if(baryon > 0.)
                    mBplus += PartDens * baryon;
                else if (baryon < 0.)
                    mBminus += PartDens * baryon;

                double charge = ParticlesDBManager::Instance().getCharge(part);
                if (charge > 0.)
                    mQplus += PartDens * charge;
                else if (charge < 0.)
                    mQminus += PartDens * charge;

                double charm = ParticlesDBManager::Instance().getCharm(part);
                if (charm > 0.)
                    mCplus += PartDens * charm;
                else if (charm < 0.)
                    mCminus += PartDens * charm;

                double beauty = ParticlesDBManager::Instance().getBeauty(part);
                if (beauty > 0.)
                    mbplus += PartDens * beauty;
                else if (beauty < 0.)
                    mbminus += PartDens * beauty;
            }

            mStrange = mSplus + mSminus;
            mBaryon  = mBplus + mBminus;
            mCharge  = mQplus + mQminus;
            mCharm   = mCplus + mCminus;
            mBeauty  = mbplus + mbminus;

            return 0;
        }
    }
}

//__________________________________________________________________________
void TTMThermalModelBQ::term(double *x, double *y, uint m, uint n, double *t)
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
