// Author: Yves Schutz 24 octobre 2016
//
// The steering class to run a thermus prediction (aka ROOT macro)

#include "macroparasel.h"
#include "mainwindow.h"
#include "predictionmacro.h"

#include "main/TTMParticleSet.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>

PredictionMacro PredictionMacro::mInstance = PredictionMacro();

//__________________________________________________________________________
PredictionMacro::PredictionMacro(QObject* parent) : Macro(parent),
    mConstrainMuQ(true)
{
    // ctor
    setObjectName("Prediction Macro");
}

//__________________________________________________________________________
PredictionMacro::~PredictionMacro()
{
    // dtor
    delete mFitInfo;
    delete mParaInfo;
}

//__________________________________________________________________________
//void PredictionMacro::setConstrain()
//{
//    // option of adding constraints. b/2Q, S, C and B density

//  for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
//      if (mParaSel->isConstrained((TTMParameterSet::ParameterType)type))
//          mParaInfo->constrain((TTMParameterSet::ParameterType)type, mParaSel->getConstrainValue((TTMParameterSet::ParameterType)type));
//  }
//}

//__________________________________________________________________________
//void PredictionMacro::setFitFix()
//{
//    // fix/free the parameters

//    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
//        if (mParaSel->isFixed((TTMParameterSet::ParameterType)type))
//            mParaInfo->fix((TTMParameterSet::ParameterType)type, mParaSel->getParaValue((TTMParameterSet::ParameterType)type));
//        else
//            mParaInfo->fit((TTMParameterSet::ParameterType)type,
//                           mParaSel->getParaValue((TTMParameterSet::ParameterType)type),
//                           mParaSel->getFitMin((TTMParameterSet::ParameterType)type),
//                           mParaSel->getFitMax((TTMParameterSet::ParameterType)type),
//                           mParaSel->getFitSte((TTMParameterSet::ParameterType)type));
//    }
//}

//__________________________________________________________________________
void PredictionMacro::setMacroDefaultParameters()
{
    // set the default values for the macro parameters
    mMacroParaSel->setModelBSQ();
    mMacroParaSel->setQstat();
    mMacroParaSel->setWidth();

    mMacroParaSel->updateDisplay();
}

//__________________________________________________________________________
//void PredictionMacro::setMacroParaSel(MacroParaSel *val)
//{
//    // set the MacroParasel wizard page and set default parameters
//    mMacroParaSel = val;
//    setMacroDefaultParameters();
//    mMacroParaSel->updateDisplay();
//}

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
//void PredictionMacro::setFit()
//{
//    // sets the fit model

//    QString name = mMacroParaSel->dataFileName();
//    if (name.isEmpty()) {
//        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
//        msg.setInformativeText(tr("You did not provide a data file name. Bye!"));
//        msg.exec();
//        exit(1);
//    }
//    mFitInfo = new TTMThermalFitBSQ(mParaInfo, name, this);
//    if (mMacroParaSel->isQstat())
//        mFitInfo->setQStats(true);
//    if (mMacroParaSel->isWidth())
//        mFitInfo->setWidth(true);
//}

//__________________________________________________________________________
//void PredictionMacro::setParameters()
//{
//    // set the parameters
//    if (mMacroParaSel->isModelBSQ())
//        mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(TTMParameterSet::kT),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUB),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUS),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUQ),
//                                           mParaSel->getParaValue(TTMParameterSet::kGAMMAS),
//                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUC),
//                                           mParaSel->getParaValue(TTMParameterSet::kGAMMAC),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUBEAUTY),
//                                           mParaSel->getParaValue(TTMParameterSet::kGAMMABEAUTY),
//                                           mParaSel->getB2Q());
//    else
//        mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(TTMParameterSet::kT),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUB),
//                                           mParaSel->getParaValue(TTMParameterSet::kMUQ),
//                                           mParaSel->getParaValue(TTMParameterSet::kGAMMAS),
//                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
//                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
//                                           mParaSel->getB2Q());


//    mParaInfo->setParent(this);

//    // choice of parameter to fit or to fix
//    setFitFix();

//    // option of adding some constraint
//    setConstrain();

//    // list all settings
//    listParameters();
//}

//__________________________________________________________________________
//void PredictionMacro::setParaSel(ParaSel *val)
//{
//    // set the Parasel wizard page and set default parameters
//    mParaSel = val;
//    setDefaultParameters();
//}

//__________________________________________________________________________
PredictionMacro &PredictionMacro::instance()
{
    // returns the unique instance

    return mInstance;
}

//__________________________________________________________________________
void PredictionMacro::run() const
{
    mFitInfo->generateYields();
    mFitInfo->listYields();
    for (TTMYield* yield : mFitInfo->getYields())
        qInfo() << Q_FUNC_INFO << "; PREDICTION: ;" <<
                   yield->getID1() << ";" <<
                   yield->getID2() << ";" <<
                   yield->getTMName() << ";" <<
                   yield->getModelValue();
}
