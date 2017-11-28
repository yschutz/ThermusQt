// Author: Yves Schutz novembre 2017
//
// Base class for the macros to run prediction or fit

#include "macroparasel.h"
#include "parasel.h"
#include "macro.h"

#include "main/TTMParameterSetBSQ.h"
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalFitBSQ.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

//__________________________________________________________________________
Macro::Macro(QObject *parent) : QObject(parent),
   mBusytics(0), mFitInfo(nullptr), mMacroParaSel(nullptr), mParaInfo(nullptr), mParaSel(nullptr)
{
    // ctor
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

//__________________________________________________________________________
void Macro::setConstrain()
{
    // option of adding constraints. b/2Q, S, C and B density

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        if (mParaSel->isConstrained((TTMParameterSet::ParameterType)type))
            mParaInfo->constrain((TTMParameterSet::ParameterType)type, mParaSel->getConstrainValue((TTMParameterSet::ParameterType)type));
    }
}

//__________________________________________________________________________
void Macro::setFit()
{
    // sets the fit model

    QString name = mMacroParaSel->dataFileName();
    if (name.isEmpty()) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(tr("You did not provide a data file name. Bye!"));
        msg.exec();
        exit(1);
    }
    //    if (mMacroParaSel->isModelBSQ())
    mFitInfo = new TTMThermalFitBSQ(mParaInfo, name, this);
    //    else
    //    mFitInfo = new TTMThermalFitBQ(this);

    if (mMacroParaSel->isQstat())
        mFitInfo->setQStats(true);
    if (mMacroParaSel->isWidth())
        mFitInfo->setWidth(true);
    if (mMacroParaSel->isExclVol())
        mFitInfo->setExclVol(true);

    QList<int> hyperons({3122, -3122, 3312, -3312, 3334, -3334});
    QList<int> nuclei({1000010020, -1000010020, 1000020030, 1010010030, -1010010030});
    QList<int> proton({2212, -2212});
    QList<int> resonances({313, 333, 3124});

    if (mMacroParaSel->isYields()) {
        if (!mMacroParaSel->isNucleiFitted()) {
            for (int id : nuclei)
               mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
        if (!mMacroParaSel->isHyperonsFitted()) {
            for (int id : hyperons)
               mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
        if (!mMacroParaSel->isProtonFitted()) {
            for (int id : proton)
               mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
        if (!mMacroParaSel->isResonancesFitted()) {
            for (int id : resonances)
               mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
    }
}

//__________________________________________________________________________
void Macro::setFitFix()
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
void Macro::setMacroParaSel(MacroParaSel *val)
{
    mMacroParaSel = val;
    setMacroDefaultParameters();
}

//__________________________________________________________________________
void Macro::setParameters()
{
    // set the parameters
    //    if (mMacroParaSel->isModelBSQ())
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
    //    else
    //        mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(TTMParameterSet::kT),
    //                                           mParaSel->getParaValue(TTMParameterSet::kMUB),
    //                                           mParaSel->getParaValue(TTMParameterSet::kMUQ),
    //                                           mParaSel->getParaValue(TTMParameterSet::kGAMMAS),
    //                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
    //                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
    //                                           mParaSel->getB2Q());

    mParaInfo->setParent(this);

    // choice of parameter to fit or to fix
    setFitFix();

    // option of adding some constraint
    setConstrain();

    // list all settings
    listParameters();
    mMacroParaSel->list();
}

//__________________________________________________________________________
void Macro::setParaSel(ParaSel *val)
{
    // set the Parasel wizard page and set default parameters
    mParaSel = val;
    setDefaultParameters();
}

//__________________________________________________________________________
void Macro::start(bool debug)
{
    setDebug(debug);
    QDateTime start = QDateTime::currentDateTime();
    QDate date = start.date();
    QTime time = start.time();

    QString info = QString(" *** Start at : %1 Time : %2").arg(date.toString("dd MMMM yyyy")).arg(time.toString());
    qInfo() << info;
}
