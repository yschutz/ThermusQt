#include "macroparasel.h"
#include "parasel.h"
#include "xxxxxxxx.h"

//__________________________________________________________________________
void XxXxxxxx::run(const QString &message)
{
    localInit(); // do not remove

    if (!isInitialized()) {
        QMessageBox::information(nullptr, Q_FUNC_INFO, "intitialization failed");
    }
    else {
        qInfo() << "******* Fitting:" << mMacroParaSel->getTitle() << "******* ";

        mFitInfo->generateYields();

        mFT = new FittingThread(mFitInfo);
        connect(mFT, &FittingThread::resultReady, this, [this]{wrapUp();});
        mBusy = new QMessageBox(QMessageBox::Information, Q_FUNC_INFO, "FIT");
        mBusy->setText(QString("%1: is fitting").arg(Q_FUNC_INFO));
        mBusy->setStandardButtons(QMessageBox::Abort);
        mBusy->setInformativeText("Busy");
        mTimer->start(2500);
        mFT->start();
        int ret = mBusy->exec();
        switch (ret) {
        case QMessageBox::Abort:
            mFT->blockSignals(true);
            mFT->terminate();
            mTimer->stop();
            break;
        case QMessageBox::Ok:
            mBusy->close();
            break;
        default:
            break;
        }
    }
}


//__________________________________________________________________________
void XxXxxxxx::localInit()
{
    bool rv = false;
    setObjectName("XxXxxxxx");

    // set default thermal model parameters; can be modified interactively
    double Tch         = 0.156;  // GeV
    double muB         = 0.0001; // GeV
    double muS         = 0.00;   // GeV
    double muQ         = 0.0;    // GeV
    double gammaS      = 1.00;   // 0.86
    double radius      = 10.5;   // units = fm but no dependence for Grand Canonical.
                                 // R=10.9 fm give 5425 fm^3 and R=10.5 fm give 4850 fm^3
    double cradius     = 0.0;
    double muC         = 0.0;
    double gammaC      = 1.0;
    double muBeauty    = 0.0;
    double gammaBeauty = 1.0;

    // set default fitting model; can be modified interactively
    bool bsq     = true;    // thermodynamical model bq (canonical) = false or bsq (grand canonical) = true
    bool qstat   = true;    // apply quantum statistics
    bool width   = true;    // apply resonance width
    bool exclvol = false;   // apply exclusion volume

    bool fitHyp  = true;    // fit hyperons ?
    bool fitPro  = true;    // fit protons ?
    bool fitRes  = false;   // fit resonances ?
    bool fitNuc  = false;   // fit nuclei ?

    mHyperons.append({3122, -3122, 3312, -3312, 3334, -3334}); // Lambda, Lambda_bar, Ksi-, Ksi+, Omega, Omega_bar
    mNuclei.append({1000010020, -1000010020, 1000020030, 1010010030, -1010010030}); // Deuteron, Deuteron_bar, He3, Hypertriton, Hypertriton_bar
    mProton.append({2212, -2212}); // proton, proton_bar
    mResonances.append({313, 333, 3124}); // K*(892), phi(1020), Lambda(1520)

    QString data = "";      // file of data to be fitted


    //BEG: do not remove
    rv = init(Tch, muB, muS, muQ, gammaS, radius, cradius, muC, gammaC, muBeauty, gammaBeauty,
              bsq, qstat, width, exclvol, fitHyp, fitPro, fitRes, fitNuc, data);
    mInitialized = rv;
    //END: do not remove
}
