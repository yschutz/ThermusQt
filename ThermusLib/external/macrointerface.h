// Author: Yves Schutz February 2018
//
// Base class for the macros to run prediction or fit

#ifndef MACROINTERFACE_H
#define MACROINTERFACE_H

#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QTimer>

#include "TTMThermalFitBSQ.h"
#include "TTMParameterSetBSQ.h"

#include "fittingthread.h"

class MacroParaSel;
class ParaSel;


class MacroInterface {
public:
            bool init(double Tch, double muB, double muS, double muQ, double gammaS, double radius,
                      double cradius, double mu, double gamma, double muBeauty, double gammaBeauty,
                      bool bsq, bool qstat, bool width, bool exclvol,
                      bool fitHyp, bool fitPro, bool fitRes, bool fitNuc, const QString & data);
            bool isDebug() const           { return mDebug; }
            bool isInitialized() const     { return mInitialized; }
            void listParameters() const    { mParaInfo->list(); }
    virtual void localInit()               = 0;
    virtual void run()                     = 0;
            void setConstrain();
            void setDebug(bool val)        { mDebug = val; }
            void setFit();
            void setFitFix();
            void setParameters();

public slots:
            void timeout();
            void wrapUp();

protected:
    QMessageBox*           mBusy;                // displayed when process going on
    bool                   mInitialized = false; // flag if initialization done or not
    int                    mBusytics;            // timer ticks
    bool                   mDebug;               // true if debug mode on
    TTMThermalFitBSQ*      mFitInfo;             // the fit model to be used
    FittingThread*         mFT;                  // the thread where the fitting process is running
    QList<int>             mHyperons;            // list of hyperons to be fitted
    MacroParaSel*          mMacroParaSel;        // window for parameters selection
    QList<int>             mNuclei;              // list of nuclei to be fitted
    TTMParameterSetBSQ*    mParaInfo;            // holds the information on parameters
    ParaSel*               mParaSel;             // window for parameters selection
    QList<int>             mProton;              // list of protons to be fitted
    QList<int>             mResonances;          // list of resonances to be fitted
    QTimer*                mTimer;               // a timer
};

QT_BEGIN_NAMESPACE

#define MacroInterFace_iid "aaaa.bbbbb.MacroInterface"

Q_DECLARE_INTERFACE(MacroInterface, MacroInterFace_iid)

QT_END_NAMESPACE

#endif // MACROINTERFACE_H
