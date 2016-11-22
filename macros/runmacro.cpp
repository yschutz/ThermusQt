// Author: Yves Schutz 24 octobre 2016
//
// The steering class to run a thermus prediction(aka ROOT macro)

#include "filedialog.h"
#include "parasel.h"
#include "runmacro.h"

#include "main/TTMparticleSet.h"

#include <QDebug>
#include <QDir>

RunMacro RunMacro::mInstance = RunMacro();

//__________________________________________________________________________
RunMacro::RunMacro() ://mFileDialog(NULL),
    mParaInfo(NULL),
    mParaSel(NULL), mPartInfo(NULL)
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
    // option of addin constraints. b/2Q, S, C and B density

  for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
      if (mParaSel->isConstrained((ParaSel::ParameterType)type))
          mParaInfo->constrain((ParaSel::ParameterType)type, mParaSel->getConstrainValue((ParaSel::ParameterType)type));
  }
}

//__________________________________________________________________________
void RunMacro::setFitFix()
{
    // fix/free the parameters

    for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
        if (mParaSel->isFixed((ParaSel::ParameterType)type))
            mParaInfo->fix((ParaSel::ParameterType)type, mParaSel->getParaValue((ParaSel::ParameterType)type));
        else
            mParaInfo->fit((ParaSel::ParameterType)type,
                           mParaSel->getParaValue((ParaSel::ParameterType)type),
                           mParaSel->getFitMin((ParaSel::ParameterType)type),
                           mParaSel->getFitMax((ParaSel::ParameterType)type),
                           mParaSel->getFitSte((ParaSel::ParameterType)type));
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

    double radiusDefault = 10.5; //units = fm but no dependence for Grand Canonical.
    // R=10.9 fm give 5425 fm^3 and R=10.5 fm give 4850 fm^3

    double muCDefault      = 0.0;
    double gammaCDefault   = 1.0;
    double muBeautyDefault = 0.0;
    double gammaBeautyDefault = 0.0;

//    double BDefault = 208;  // 197
//    double QDefault  = 82;  //  79

    // ==== Default parameters =======

    mParaSel->setParaValue(ParaSel::kTemp, TchDefault);
    mParaSel->setParaValue(ParaSel::kMuQ, muQDefault);
    mParaSel->setParaValue(ParaSel::kMuB, muBDefault);
    mParaSel->setParaValue(ParaSel::kMuS, muSDefault);
    mParaSel->setParaValue(ParaSel::kMuC, muCDefault);
    mParaSel->setParaValue(ParaSel::kGammaS, gammaSDefault);
    mParaSel->setParaValue(ParaSel::kGammaC, gammaCDefault);
    mParaSel->setParaValue(ParaSel::kRadius, radiusDefault);
    mParaSel->setParaValue(ParaSel::kMuBeauty, muBeautyDefault);
    mParaSel->setParaValue(ParaSel::kGammaBeauty, gammaBeautyDefault);

    double fitMin = 0.05;
    double fitMax = 0.180;
    double fitSte = 0.001;

    for (qint32 type = 0; type < ParaSel::kParTypes; type++)
        mParaSel->setFitValues((ParaSel::ParameterType)type, fitMin, fitMax, fitSte);
   mParaSel->updateDisplay();
}

//__________________________________________________________________________
void RunMacro::setParameters()
{
    // set the parameters
    mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(ParaSel::kTemp),
                           mParaSel->getParaValue(ParaSel::kMuB),
                           mParaSel->getParaValue(ParaSel::kMuS),
                           mParaSel->getParaValue(ParaSel::kMuQ),
                           mParaSel->getParaValue(ParaSel::kGammaS),
                           mParaSel->getParaValue(ParaSel::kRadius),
                           mParaSel->getParaValue(ParaSel::kMuC),
                           mParaSel->getParaValue(ParaSel::kGammaC),
                           mParaSel->getParaValue(ParaSel::kMuBeauty),
                           mParaSel->getParaValue(ParaSel::kGammaBeauty)
                           );
    mParaInfo->setParent(this);
    mParaInfo->setB2Q(mParaSel->getB2Q());
}

//__________________________________________________________________________
void RunMacro::setParaSel(ParaSel *val)
{
    // set the Parasel wizard page and set default parameters
    mParaSel = val;
    setDefaultParameters();
}

//__________________________________________________________________________
void RunMacro::setParticlesListFile()
{
    // setting the particles list and particles properties

    mParticlesList = mFileDialog->getFileName();
    mParticlesList.prepend(":/particles/");
    QString tempo(QString("The selected particles lis file is: %1").arg(mParticlesList));
    if (mDebug)
        qDebug() << Q_FUNC_INFO << tempo;

    mPartInfo = new  TTMParticleSet(mParticlesList, true);  // here true means the decays are scaled to sum(BR) = 100%
    mPartInfo->inputDecays(":/particles/");
}

//__________________________________________________________________________
RunMacro &RunMacro::instance()
{
    // returns the unique instance

    return mInstance;

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
