// Author: Yves Schutz novembre 2017
//
// The steering class to run a thermus fit (aka ROOT macro)

#include "fitmacro.h"
#include "macroparasel.h"
#include "main/TTMThermalFitBSQ.h"


FitMacro FitMacro::mInstance = FitMacro();

//__________________________________________________________________________
FitMacro &FitMacro::instance()
{
    // returns the unique instance

    return mInstance;
}

//__________________________________________________________________________
void FitMacro::setDefaultParameters()
{
    // ==== Starting parameters ======= (nucl-ex/0403014)

    double TchStart    =  0.150;   // GeV
    double muBStart    =  0.0001;  // GeV
    double muSStart    =  0.0;     // GeV
    double muQStart    =  0.0;     // GeV

    double gammaSStart =  1.0;   // 0.86

    double radiusStart  = 3.0;    // units = fm but no dependence for Grand Canonical.
    double cradiusStart = 0.0;

    double muCStart         = 0.0;
    double gammaCStart      = 1.0;
    double muBeautyStart    = 0.0;
    double gammaBeautyStart = 0.0;

    // ==== Start parameters =======

    mParaSel->setParaValue(TTMParameterSet::kT,           TchStart);
    mParaSel->setParaValue(TTMParameterSet::kMUQ,         muQStart);
    mParaSel->setParaValue(TTMParameterSet::kMUB,         muBStart);
    mParaSel->setParaValue(TTMParameterSet::kMUS,         muSStart);
    mParaSel->setParaValue(TTMParameterSet::kMUC,         muCStart);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAS,      gammaSStart);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAC,      gammaCStart);
    mParaSel->setParaValue(TTMParameterSet::kRADIUS,      radiusStart);
    mParaSel->setParaValue(TTMParameterSet::kCRADIUS,     cradiusStart);
    mParaSel->setParaValue(TTMParameterSet::kMUBEAUTY,    muBeautyStart);
    mParaSel->setParaValue(TTMParameterSet::kGAMMABEAUTY, gammaBeautyStart);

    mParaSel->setFitValues(TTMParameterSet::kT, 0.130, 0.190, 0.0001);
    mParaSel->fixParameter(TTMParameterSet::kMUB);
    mParaSel->fixParameter(TTMParameterSet::kGAMMAS);
    mParaSel->fixParameter(TTMParameterSet::kGAMMABEAUTY);
    mParaSel->setFitValues(TTMParameterSet::kRADIUS, 1.0, 15.0, 0.01);
    mParaSel->fixParameter(TTMParameterSet::kMUQ);
    mParaSel->fixParameter(TTMParameterSet::kMUS);

    mParaSel->constrainParameter(TTMParameterSet::kMUQ);
    mParaSel->updateDisplay();
}

//__________________________________________________________________________
void FitMacro::setMacroDefaultParameters()
{
    // set additional parameters
    bool   bsq    = true;    // thermodynamical model bq (canonical) or bsq (grand canonical)
    bool   qstat  = true;    // apply quantum statistics
    bool   width  = true;    // apply resonance width
    double radius = 0.3;     // radius for the exclusion volume

    // set the default values for the macro parameters

    mMacroParaSel->setModelBSQ(bsq);
    mMacroParaSel->setQstat(qstat);
    mMacroParaSel->setWidth(width);
    mMacroParaSel->setExclVol(radius);
}

//__________________________________________________________________________
void FitMacro::run() const
{
    qInfo() << "******* Fitting:" << mMacroParaSel->getTitle() << "******* ";

    mFitInfo->generateYields();
    mFitInfo->listYields();

    for (TTMYield* yield : mFitInfo->getYields())
        qInfo() << Q_FUNC_INFO << "; PREDICTION: ;" <<
                   yield->getID1() << ";" <<
                   yield->getID2() << ";" <<
                   yield->getTMName() << ";" <<
                   yield->getModelValue();
    mFitInfo->fitData(0);
}

//__________________________________________________________________________
FitMacro::FitMacro(QObject* parent) : Macro(parent)
{
    // ctor
    setObjectName("Fit Macro");
}

//__________________________________________________________________________
FitMacro::~FitMacro()
{

}
