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

#include "TTMThermalModel.h"

class TTMParameterSetBQ;

class TTMThermalModelBQ : public TTMThermalModel
{
public:
    TTMThermalModelBQ(QObject* parent = 0);
    TTMThermalModelBQ(TTMParameterSetBQ *parameters, bool width = true, QObject* parent = 0);

    int generateParticleDens();

private:
    void Term(double *x, double *y, uint m, uint n, double *t);

    bool               mNonStrangeQStats;	// true if S=0 hadrons are to be treated
    TTMParameterSetBQ* mPar;                // the parameters set
};

#endif // TTMTHERMALMODELBQ_H
