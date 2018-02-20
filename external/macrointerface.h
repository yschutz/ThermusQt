#ifndef MACROINTERFACE_H
#define MACROINTERFACE_H

#include <QMessageBox>
#include <QString>

#include <main/TTMParameterSetBSQ.h>

class MacroParaSel;
class ParaSel;
class TTMThermalFitBSQ;

class QTimer;

class MacroInterface
{
public:
    virtual bool    init() = 0;
            bool    isInitialized() const { return mInitialized; }
    virtual QString run(const QString& message) = 0;

protected:
    bool                   mInitialized = false; // flag if initialization done or not
    QMessageBox*           mBusy;                // displayed when process going on
    int                    mBusytics;            // timer ticks
    bool                   mDebug;               // true if debug mode on
    TTMThermalFitBSQ*      mFitInfo;             // the fit model to be used
    MacroParaSel*          mMacroParaSel;        // window for parameters selection
    TTMParameterSetBSQ*    mParaInfo;            // holds the information on parameters
    ParaSel*               mParaSel;             // window for parameters selection
    QTimer*                mTimer;               // a timer
};

QT_BEGIN_NAMESPACE

#define MacroInterFace_iid "aaaa.bbbbb.MacroInterface"

Q_DECLARE_INTERFACE(MacroInterface, MacroInterFace_iid)

QT_END_NAMESPACE

#endif // MACROINTERFACE_H
