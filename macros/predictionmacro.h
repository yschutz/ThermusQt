// Author: Yves Schutz 24 octobre 2016
//
// The steering class to prediction thermus (aka ROOT macro)
// Singleton


#ifndef PREDICTION_H
#define PREDICTION_H

#include <QObject>

#include "macro.h"

class ParaSel;
class MacroParaSel;
class PredictionMacro : public Macro
{
    Q_OBJECT

public:
    static      PredictionMacro& instance();

    TTMParameterSet*    getParametersSet() const  { return mParaInfo; }
    void                run();
    void                setDefaultParameters();
    void                setMacroDefaultParameters();

public slots:
    void timeout() {;}

private:
    PredictionMacro(QObject* parent = nullptr);
    ~PredictionMacro();
    PredictionMacro (const PredictionMacro& ma) : Macro(ma.parent()) {;}

    bool                   mConstrainMuQ;     // true if B/2Q to be constrained
    static PredictionMacro mInstance;         // unique instance of the object
};

#endif // PREDICTION_H
