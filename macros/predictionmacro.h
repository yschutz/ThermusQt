// Author: Yves Schutz 24 octobre 2016
//
// The steering class to prediction thermus (aka ROOT macro)
// Singleton


#ifndef PREDICTION_H
#define PREDICTION_H

#include <QDebug>
#include <QObject>

#include "main/TTMParticleSet.h"
#include "main/TTMParameterSetBSQ.h"
#include "main/TTMThermalFitBSQ.h"

#include "macros/macro.h"

class ParaSel;
class MacroParaSel;
class PredictionMacro : public Macro
{
    Q_OBJECT

public:
    static      PredictionMacro& instance();

    TTMParameterSet*    getParametersSet() const  { return mParaInfo; }
//    bool                isDebug() const           { return mDebug; }
//    void                listParameters() const    { mParaInfo->list(); }
    void                run() const;
//    void                setConstrain();
//    void                setDebug(bool val)        { mDebug = val; }
    void                setDefaultParameters();
//    void                setFit();
//    void                setFitFix() ;
    void                setMacroDefaultParameters();
//    void                setMacroParaSel(MacroParaSel* val);
//    void                setParameters();
//    void                setParaSel(ParaSel* val);

private:
    PredictionMacro(QObject* parent = nullptr);
    ~PredictionMacro();
    PredictionMacro (const PredictionMacro&) {;}

    bool                   mConstrainMuQ;     // true if B/2Q to be constrained
//    bool                   mDebug;            // true if debug mode on
//    TTMThermalFitBSQ*      mFitInfo;          // the fit model to be used
    static PredictionMacro mInstance;         // unique instance of the object
//    MacroParaSel*          mMacroParaSel;     // window for parameters selection
//    TTMParameterSetBSQ*    mParaInfo;         // holds the information on parameters
//    ParaSel*               mParaSel;          // window for parameters selection
};

#endif // PREDICTION_H
