// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//////////////////////////////////////////////////////////////////////////
//                                                                 	    //
// TTMThermalModelBQ							                        //
//            	                                 			            //
// Strangeness Canonical Thermal Model Class				            //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALMODELBQ_H
#define TTMTHERMALMODELBQ_H

#include "TTMParameterSetBQ.h"
#include "TTMThermalModel.h"

class TTMThermalModelBQ : public TTMThermalModel
{
public:
    TTMThermalModelBQ(QObject* parent = nullptr);
    TTMThermalModelBQ(TTMParameterSetBQ *parameters, bool width = true, QObject* parent = nullptr);
    TTMThermalModelBQ(const TTMThermalModelBQ& model);
    ~TTMThermalModelBQ();

    int                constrainEoverN(double eovern);
    int                constrainNetBaryonDensity(double nb);
    int                constrainPercolation();
    int                constrainSoverT3(double soverT3);
    int                constrainTotalBaryonDensity(double nb);
    void               generateEnergyDens();
    void               generateEntropyDens();
    int                generateParticleDens();
    void               generatePressure();
    TTMParameterSetBQ* getParameterSet() const { return mPar; }
    void               listInfo() const;
    int                primPartDens();
    void               reset();

    TTMThermalModelBQ& operator=(TTMThermalModelBQ model);

private:
    void term(double *x, double *y, uint m, uint n, double *t);

    double             mCorrM1; 	        // canonical correction factor for S=-1 particles
    double             mCorrM2; 	        // canonical correction factor for S=-2 particles
    double             mCorrM3;	            // canonical correction factor for S=-3 particles
    double             mCorrP1; 	        // canonical correction factor for S=+1 particles
    double             mCorrP2; 	        // canonical correction factor for S=+2 particles
    double             mCorrP3;	            // canonical correction factor for S=+3 particles
    double             mExactMuS;           // exact strangeness chemical potential
    double             mlnZtot;	            // log(total partition function)
    double             mlnZ0;	            // log(non-strange part of partition function)
    bool               mNonStrangeQStats;	// true if S=0 hadrons are to be treated
    TTMParameterSetBQ* mPar;                // the parameters set

};
#endif // TTMTHERMALMODELBQ_H
