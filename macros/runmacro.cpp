// Author: Yves Schutz 24 octobre 2016
//
// The steering class to run a thermus prediction(aka ROOT macro)

//#include "filesel.h"
#include "mainwindow.h"
#include "parasel.h"
#include "runmacro.h"

#include "main/TTMParticleSet.h"

#include <QDebug>
#include <QDir>

RunMacro RunMacro::mInstance = RunMacro();

//__________________________________________________________________________
RunMacro::RunMacro() :
    mConstrainMuQ(true), mFitInfo(nullptr), mParaInfo(nullptr), mParaSel(nullptr), mQuantRes(true)
{
    // ctor
    setObjectName("Run Macro");

}

//__________________________________________________________________________
RunMacro::~RunMacro()
{
    // dtor
}

//__________________________________________________________________________
void RunMacro::setConstrain()
{
    // option of adding constraints. b/2Q, S, C and B density

  for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
      if (mParaSel->isConstrained((TTMParameterSet::ParameterType)type))
          mParaInfo->constrain((TTMParameterSet::ParameterType)type, mParaSel->getConstrainValue((TTMParameterSet::ParameterType)type));
  }
}

//__________________________________________________________________________
void RunMacro::setFitFix()
{
    // fix/free the parameters

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        if (mParaSel->isFixed((TTMParameterSet::ParameterType)type))
            mParaInfo->fix((TTMParameterSet::ParameterType)type, mParaSel->getParaValue((TTMParameterSet::ParameterType)type));
        else
            mParaInfo->fit((TTMParameterSet::ParameterType)type,
                           mParaSel->getParaValue((TTMParameterSet::ParameterType)type),
                           mParaSel->getFitMin((TTMParameterSet::ParameterType)type),
                           mParaSel->getFitMax((TTMParameterSet::ParameterType)type),
                           mParaSel->getFitSte((TTMParameterSet::ParameterType)type));
    }
}

//__________________________________________________________________________
void RunMacro::setDefaultParameters()
{
    // Select parameters from an interactive window

    // ==== Default parameters ======= (nucl-ex/0403014)
    double TchDefault = 0.156;  // GeV
    double muBDefault = 0.0001; // GeV
    double muSDefault = 0.00;   // GeV
    double muQDefault = 0.0;    // GeV

    double gammaSDefault = 1.00; // 0.86

    double radiusDefault  = 10.5; //units = fm but no dependence for Grand Canonical.
    // R=10.9 fm give 5425 fm^3 and R=10.5 fm give 4850 fm^3
    double cradiusDefault = 0.0;

    double muCDefault      = 0.0;
    double gammaCDefault   = 1.0;
    double muBeautyDefault = 0.0;
    double gammaBeautyDefault = 1.0;

//    double BDefault = 208;  // 197
//    double QDefault  = 82;  //  79

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

    double fitMin = 0.05;
    double fitMax = 0.180;
    double fitSte = 0.001;

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++)
        mParaSel->setFitValues((TTMParameterSet::ParameterType)type, fitMin, fitMax, fitSte);
    mParaSel->updateDisplay();
}

//__________________________________________________________________________
void RunMacro::setFit()
{
    // sets the fit model

    mFitInfo = new TTMThermalFitBSQ(mParaInfo, "prediction_yannick.txt", this);
    // -> Turn off default quantum statistics and resonance width treatment
    if (!mQuantRes) {
        mFitInfo->setQStats(true);
        mFitInfo->setWidth(true);
    }
}

//__________________________________________________________________________
void RunMacro::setParameters()
{
    // set the parameters
    qDebug() << Q_FUNC_INFO << mParaSel->getParaValue(TTMParameterSet::kMUQ);

    mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(TTMParameterSet::kT),
                                       mParaSel->getParaValue(TTMParameterSet::kMUB),
                                       mParaSel->getParaValue(TTMParameterSet::kMUS),
                                       mParaSel->getParaValue(TTMParameterSet::kMUQ),
                                       mParaSel->getParaValue(TTMParameterSet::kGAMMAS),
                                       mParaSel->getParaValue(TTMParameterSet::kRADIUS),
                                       mParaSel->getParaValue(TTMParameterSet::kMUC),
                                       mParaSel->getParaValue(TTMParameterSet::kGAMMAC),
                                       mParaSel->getParaValue(TTMParameterSet::kMUBEAUTY),
                                       mParaSel->getParaValue(TTMParameterSet::kGAMMABEAUTY),
                                       mParaSel->getB2Q());
    mParaInfo->setParent(this);

    // choice of parameter to fit or to fix
    setFitFix();

    // option of adding some constraint
    setConstrain();

    // list all settings
    listParameters();
}

//__________________________________________________________________________
void RunMacro::setParaSel(ParaSel *val)
{
    // set the Parasel wizard page and set default parameters
    mParaSel = val;
    setDefaultParameters();
}

//__________________________________________________________________________
//void RunMacro::setParticlesListFile()
//{
//    // setting the particles list and particles properties

//    mParticlesList = mFileSel->getFileName();
//    mParticlesList.prepend(":/particles/");
//    QString tempo(QString("The selected particles lis file is: %1").arg(mParticlesList));
//    if (mDebug)
//        qDebug() << tempo;

//    mPartInfo = new  TTMParticleSet(mParticlesList, true);  // here true means the decays are scaled to sum(BR) = 100%
//    mPartInfo->inputDecays(":/particles/");
//}

//__________________________________________________________________________
RunMacro &RunMacro::instance()
{
    // returns the unique instance

    return mInstance;
}

//__________________________________________________________________________
void RunMacro::run() const
{
    mFitInfo->generateYields();
//    mFitInfo->listYields();
//    TTMYield* yield = mFitInfo->getYield(211, 0, "PREDICTION");
//    qInfo() << yield->getID1() << yield->getTMName() << yield->getModelValue();
}

//__________________________________________________________________________
//void RunMacro::init(QString name)
//{
//    // Equivalent to root macro to prediction Thermus
//    setObjectName(name);

//    // **************************************************
//    // First, definition the particle list
//    // and their properties (and decays):
//    // open the particles list file

//    if (!selectParticlesListFile())
//        qFatal("no particles list file selected");

//    qDebug() << Q_FUNC_INFO << "INFO:" << mParticlesList << "selected";
//    TTMParticleSet set(mParticlesList, true);
//    set.inputDecays(":/particles/");

//    // **************************************************
//    // Second, choice of formalism:
//    // - We want here a Grand Canonical Treatment so we use
//    //  a TTMThermalFitBSQ as a FIT instance with starting parameters
//    //  other FIT classes are:
//    // -> TTMThermalFitBQ for Strangeness Canonical;
//    // -> TTMThermalFitCanBSQ for Full Canonical \n");
//    //
//    // Third, choice of starting parameters (nucl-ex/0403014)

//    if (!selectDefaultParameters())
//        qFatal("no parameters selected");
//}
