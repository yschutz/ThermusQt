// Author: Yves Schutz 24 octobre 2016
//
// The steering class to prediction thermus (aka ROOT macro)
// Singleton


#ifndef PREDICTION_H
#define PREDICTION_H

#include <QDebug>
#include <QObject>

//#include "filesel.h"
//#include "parasel.h"
#include "summary.h"

#include "main/TTMParticleSet.h"
#include "main/TTMParameterSetBSQ.h"
#include "main/TTMThermalFitBSQ.h"

class RunMacro : public QObject
{
    Q_OBJECT

public:
    static      RunMacro& instance();

    TTMParameterSetBSQ* getParametersSet() const  { return mParaInfo; }
    bool                isDebug() const           { return mDebug; }
    void                listParameters() const    { mParaInfo->list(); }
    void                run() const;
    void                setConstrain();
    void                setDebug(bool val)        { mDebug = val; }
    void                setDefaultParameters();
    void                setFit();
    void                setFitFix() ;
    void                setParameters();
    void                setParaSel(ParaSel* val);
    void                setSummary(Summary * val) { mSummary = val; }

private:
    RunMacro();
    ~RunMacro();
    RunMacro (const RunMacro&) { }
//    prediction& operator = (const prediction&) { }

    bool                mConstrainMuQ;     // true if B/2Q to be constrained
    bool                mDebug;            // true if debug mode on
    TTMThermalFitBSQ*   mFitInfo;          // the fit model to be used
    static RunMacro     mInstance;         // unique instance of the object
    TTMParameterSetBSQ* mParaInfo;         // holds the information on parameters
    ParaSel*            mParaSel;          // window for parameters selection
    bool                mQuantRes;
    Summary*            mSummary;          // summary wizard page
};

#endif // PREDICTION_H
