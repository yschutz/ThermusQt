// Author: Yves Schutz novembre 2017
//
// Base class for the macros to run prediction or fit
#ifndef MACRO_H
#define MACRO_H

#include <QObject>

#include <main/TTMParameterSetBSQ.h>

class MacroParaSel;
class ParaSel;
class TTMThermalFitBSQ;

class Macro : public QObject
{
    Q_OBJECT
public:
    explicit Macro(QObject *parent = nullptr);

    bool                isDebug() const           { return mDebug; }
    void                listParameters() const    { mParaInfo->list(); }
    void                setConstrain();
    void                setDebug(bool val)        { mDebug = val; }
    virtual void        setDefaultParameters() = 0;
    void                setFit();
    void                setFitFix();
    virtual void        setMacroDefaultParameters() = 0;
    void                setMacroParaSel(MacroParaSel* val);
    virtual void        setParameters();
    virtual void        run () const           = 0;
    void                setParaSel(ParaSel* val);


    void                start(bool debug = false);

protected:
    bool                   mDebug;            // true if debug mode on
    TTMThermalFitBSQ*      mFitInfo;          // the fit model to be used
    MacroParaSel*          mMacroParaSel;     // window for parameters selection
    TTMParameterSetBSQ*    mParaInfo;         // holds the information on parameters
    ParaSel*               mParaSel;          // window for parameters selection

};

#endif // MACRO_H
