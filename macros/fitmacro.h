// Author: Yves Schutz novembre 2017
//
// The steering class to run a thermus fit (aka ROOT macro)

#ifndef FITMACRO_H
#define FITMACRO_H

#include "macro.h"

#include <QObject>

class FittingThread;
class FitMacro : public Macro
{
    Q_OBJECT
public:
    static FitMacro& instance();
    void             setDefaultParameters();
    void             setMacroDefaultParameters();
    void             run();

public slots:
    void wrapUp();
    void timeout();

private:
    FitMacro(QObject* parent = nullptr);
    ~FitMacro();
    FitMacro (const FitMacro& ma) : Macro(ma.parent()) {;}

    FittingThread*  mFT;               // the thread where the fitting process is running
    static FitMacro mInstance;         // unique instance of the object
};

#endif // FITMACRO_H
