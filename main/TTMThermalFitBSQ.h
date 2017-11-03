//////////////////////////////////////////////////////////////////////////
//                                                                   	//
// TTMThermalFitBSQ						                            	//
//            	                                            		    //
// Fit class applying the grand canonical formalism                     //
//									                                    //
// Author: Spencer Wheaton 14 July 2004                                 //
// Adapted for Qt by Yves Schutz: October 2017                          //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALFITBSQ_H
#define TTMTHERMALFITBSQ_H

#include "TTMParameterSetBSQ.h"
#include "TTMThermalFit.h"

class TTMThermalModelBSQ;

class TTMThermalFitBSQ:public TTMThermalFit
{
public:

    TTMThermalFitBSQ(QObject* parent = nullptr);
    TTMThermalFitBSQ(TTMParameterSetBSQ* par, QString file, QObject* parent = nullptr);
    ~TTMThermalFitBSQ();

    bool                getExclVol() const                       { return mExclVol; }
    TTMParameterSetBSQ* getParameterSet() const                  { return mParm; }
    bool                getQStats() const                        { return mQStats; }
    bool                getWidth() const                         { return mWidth; }
    void                setExclVol(bool volume)                  { mExclVol = volume;}
    void                setParameterSet(TTMParameterSetBSQ* par) { mParm    = par; }
    void                setQStats(bool qstats)                   { mQStats  = qstats; }
    void                setWidth(bool width)                     { mWidth   = width; }

protected:
    TTMThermalModel* generateThermalModel();

    bool                mExclVol;  // exclusion volume applied or not
    TTMThermalModelBSQ* mModel;    // the grand canonical model
    TTMParameterSetBSQ* mParm;     // the parameters set
    bool                mQStats;   // quantum statistics applied or not
    bool                mWidth;    // resonance width applied or not
};

#endif // TTMTHERMALFITBSQ_H
