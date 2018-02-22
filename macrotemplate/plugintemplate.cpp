#include "macroparasel.h"
#include "parasel.h"
#include "xxxxxxxx.h"

//__________________________________________________________________________
void XxXxxxxx::run(const QString &message)
{
    if (!isInitialized()) {
        QMessageBox::information(nullptr, Q_FUNC_INFO, "no intitialization done");
    }
    else
        QMessageBox::information(nullptr, "Run result", QString("%1 : %2 --> %3").arg(objectName(), message, "Hello World"));
}


//__________________________________________________________________________
bool XxXxxxxx::init()
{
    bool rv = false;
    setObjectName("XxXxxxxx");
    rv = true;

    mInitialized = rv;
    return rv;
}

//__________________________________________________________________________
void XxXxxxxx::setDefaultParameters()
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

//    mParaSel->updateDisplay();
}

//__________________________________________________________________________
void XxXxxxxx::setMacroDefaultParameters()
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
