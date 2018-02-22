// Author: Yves Schutz 24 octobre 2016
//
// The steering class to run a thermus prediction (aka ROOT macro)

#include "macroparasel.h"
//#include "mainwindow.h"
#include "predictionmacro.h"

#include "TTMThermalFitBSQ.h"
#include "TTMParticleSet.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

PredictionMacro PredictionMacro::mInstance = PredictionMacro();

//__________________________________________________________________________
PredictionMacro::PredictionMacro(QObject* parent) : Macro(parent),
    mConstrainMuQ(true)
{
    // ctor
    setObjectName("Prediction_Macro_");
}

//__________________________________________________________________________
PredictionMacro::~PredictionMacro()
{
    // dtor
    delete mFitInfo;
    delete mParaInfo;
}

//__________________________________________________________________________
void PredictionMacro::setMacroDefaultParameters()
{
    // set additional parameters
    bool bsq    = true;    // thermodynamical model bq (canonical) or bsq (grand canonical)
    bool qstat  = true;    // apply quantum statistics
    bool width  = true;    // apply resonance width

    // set the default values for the macro parameters
    mMacroParaSel->setModelBSQ(bsq);
    mMacroParaSel->setQstat(qstat);
    mMacroParaSel->setWidth(width);
}

//__________________________________________________________________________
void PredictionMacro::setDefaultParameters()
{
    // ==== Default parameters ======= (nucl-ex/0403014)
    double TchDefault = 0.156;  // GeV
    double muBDefault = 0.0001; // GeV
    double muSDefault = 0.00;   // GeV
    double muQDefault = 0.0;    // GeV

    double gammaSDefault = 1.00; // 0.86

    double radiusDefault  = 10.5; //units = fm but no dependence for Grand Canonical.
    // R=10.9 fm give 5425 fm^3 and R=10.5 fm give 4850 fm^3
    double cradiusDefault = 0.0;

    double muCDefault         = 0.0;
    double gammaCDefault      = 1.0;
    double muBeautyDefault    = 0.0;
    double gammaBeautyDefault = 1.0;

    // ==== Default parameters =======

    mParaSel->setParaValue(TTMParameterSet::kT,           TchDefault);
    mParaSel->setParaValue(TTMParameterSet::kMUQ,         muQDefault);
    mParaSel->setParaValue(TTMParameterSet::kMUB,         muBDefault);
    mParaSel->setParaValue(TTMParameterSet::kMUS,         muSDefault);
    mParaSel->setParaValue(TTMParameterSet::kMUC,         muCDefault);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAS,      gammaSDefault);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAC,      gammaCDefault);
    mParaSel->setParaValue(TTMParameterSet::kRADIUS,      radiusDefault);
    mParaSel->setParaValue(TTMParameterSet::kCRADIUS,     cradiusDefault);
    mParaSel->setParaValue(TTMParameterSet::kMUBEAUTY,    muBeautyDefault);
    mParaSel->setParaValue(TTMParameterSet::kGAMMABEAUTY, gammaBeautyDefault);

    mParaSel->constrainParameter(TTMParameterSet::kMUQ);

    mParaSel->updateDisplay();
}

//__________________________________________________________________________
PredictionMacro &PredictionMacro::instance()
{
    // returns the unique instance

    return mInstance;
}

//__________________________________________________________________________
void PredictionMacro::run()
{
   QDateTime start = QDateTime::currentDateTime();
    QDate date = start.date();
    QTime time = start.time();
    QString info = QString(" *** Start at : %1 Time : %2").arg(date.toString("dd MMMM yyyy")).arg(time.toString());
    qInfo() << info;

    mFitInfo->generateYields();
    mFitInfo->listYields();
//    if (MainWindow::isVerbose()) {
//        for (TTMYield* yield : mFitInfo->getYields())
//            qInfo() << Q_FUNC_INFO << "; PREDICTION: ;" <<
//                       yield->getID1() << ";" <<
//                       yield->getID2() << ";" <<
//                       yield->getTMName() << ";" <<
//                       yield->getModelValue();
//    }
}

