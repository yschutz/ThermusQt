// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt Yves Schutz Octobre 2017
//////////////////////////////////////////////////////////////////////////
//                                                                 	    //
// TTMThermalModelBSQ                                  		         	//
//                                                                  	//
// Grand Canonical Thermal Model Class					                //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALMODELBSQ_H
#define TTMTHERMALMODELBSQ_H

#include "TTMParameterSetBSQ.h"
#include "TTMParticle.h"
#include "TTMThermalModel.h"
#include "TTMThermalParticleBSQ.h"

class TTMThermalModelBSQ : public TTMThermalModel
{
public:

    TTMThermalModelBSQ(QObject* obj = nullptr);
    TTMThermalModelBSQ(TTMParameterSetBSQ *parameters, bool qstats = true, bool width = true);
    TTMThermalModelBSQ(const TTMThermalModelBSQ& model);
    ~TTMThermalModelBSQ();


    int                 constrainBSQ(double baryon, double strange, double charge);
    int                 constrainEoverN(double eoverN);
    int                 constrainNetBaryonDensity(double nb);
    int                 constrainPercolation();
    int                 constrainQ(double charge);
    int                 constrainSoverT3(double soverT3);
    int                 constrainTotalBaryonDensity(double nb);
    double              exclVolShiftedPressure(double x);
    int                 generateParticleDens();
    void                generateEnergyDens();
    void                generateEntropyDens();
    void                generatePressure();
    bool                getExcludedVolume() const        { return mExclVolCorrection; }
    TTMParameterSetBSQ* getParameterSet() const          { return mPar; }
    void                listInfo() const;
    int                 primPartDens();
    void                setExcludedVolume(bool exVolume) { mExclVolCorrection = exVolume; }

    TTMThermalModelBSQ &operator=(TTMThermalModelBSQ model);

private:

    void                   calcExclVolPressure();

    bool                mExclVolCorrection;	  // true if excluded volume is to be taken into account
    double              mExclVolDenominator;  // denominator correction in n, e and s
    double              mExclVolPressure;     // excluded volume pressure responsible for shift in mu's
    TTMParameterSetBSQ* mPar;	              // pointer to thermal parameters
    bool                mQStats;		      // true if Quantum Stats required
};
#endif // TTMTHERMALMODELBSQ_H
