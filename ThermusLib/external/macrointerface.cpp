// Author: Yves Schutz February 2018
//
// Base class for the macros to run prediction or fit

#include "logger.h"
#include "macrointerface.h"
#include "macroparasel.h"
#include "parasel.h"
#include "thermuswiz.h"


//__________________________________________________________________________
bool MacroInterface::init()
{
    // initializes things which should be transparent to the user
    bool rv = false;
    ThermusWiz *wiz = new ThermusWiz("Custom");
    mMacroParaSel = (MacroParaSel*)wiz->page(wiz->macroparaselId());
    mParaSel      = qobject_cast<ParaSel*>(wiz->page(wiz->paraselId()));

    wiz->show();
    rv = true;

    return rv;
}

//__________________________________________________________________________
void MacroInterface::setConstrain()
{
    // option of adding constraints. b/2Q, S, C and B density

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        if (mParaSel->isConstrained((TTMParameterSet::ParameterType)type))
            mParaInfo->constrain((TTMParameterSet::ParameterType)type, mParaSel->getConstrainValue((TTMParameterSet::ParameterType)type));
    }
}

//__________________________________________________________________________
void MacroInterface::setFitFix()
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
void MacroInterface::setParameters()
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

    mParaInfo->setParent(nullptr);

    // choice of parameter to fit or to fix
    setFitFix();

    // option of adding some constraint
    setConstrain();

    // list all settings
    Logger::instance().setLogFileName("Thermus macro" + mMacroParaSel->getTitle());
    QMessageBox msg(QMessageBox::Information, Q_FUNC_INFO, Q_FUNC_INFO);
    msg.setInformativeText(QString("Log file is: %1").arg(Logger::instance().logFileName()));
    msg.exec();

    listParameters();
    mMacroParaSel->list();
}
