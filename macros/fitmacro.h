// Author: Yves Schutz novembre 2017
//
// The steering class to run a thermus fit (aka ROOT macro)

#ifndef FITMACRO_H
#define FITMACRO_H

#include "macros/macro.h"

#include <QObject>

class FitMacro : public Macro
{
    Q_OBJECT
public:
    static      FitMacro& instance();

    void                setDefaultParameters();
    void                setMacroDefaultParameters();
    void                run() const;

private:
    FitMacro(QObject* parent = nullptr);
    ~FitMacro();
    FitMacro (const FitMacro&) {;}

    static FitMacro mInstance;         // unique instance of the object


};

#endif // FITMACRO_H
