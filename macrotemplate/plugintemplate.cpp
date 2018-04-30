#include "macroparasel.h"
#include "parasel.h"
#include "plot.h"
#include "QMinuit.h"

#include "xxxxxxxx.h"

//__________________________________________________________________________
void XxXxxxxx::localInit()
{
    // set here you initialisations to perform the fit following the template below
    bool rv = false;
    setObjectName("XxXxxxxx");

    // fit or prediction

    bool fit = false;

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

    mHyperons.append({3122, -3122, 3312, -3312, 3334, -3334});                      // Lambda, Lambda_bar, Ksi-, Ksi+, Omega, Omega_bar
    mNuclei.append({1000010020, -1000010020, 1000020030, 1010010030, -1010010030}); // Deuteron, Deuteron_bar, He3, Hypertriton, Hypertriton_bar
    mProton.append({2212, -2212});                                                  // proton, proton_bar
    mResonances.append({313, 333, 3124});                                           // K*(892), phi(1020), Lambda(1520)

    QString data = "";      // file of data to be fitted

    //BEG: do not remove
    rv = init(fit, Tch, muB, muS, muQ, gammaS, radius, cradius, muC, gammaC, muBeauty, gammaBeauty,
              bsq, qstat, width, exclvol, fitHyp, fitPro, fitRes, fitNuc, data);
    mInitialized = rv;

    //END: do not remove
}

//__________________________________________________________________________
void XxXxxxxx::timeout()
{
    // refresh mBusy
    mBusy->setInformativeText(mBusy->informativeText() + '.');
    mBusytics++;
}

//__________________________________________________________________________
void XxXxxxxx::localWrapup()
{
        // actions to perform when fit is done (contour plot given as exemple below)

    const int knpoints = 50;
    const int kpa1     = 1, kpa2 = 0;
    QVector<double> xcoor(knpoints + 1);
    QVector<double> ycoor(knpoints + 1);

    Plot pl("n-σ contours", xcoor.size(), 2.5, 3.5, 0.14, 0.17);
    pl.setAxisTitle(QMinuit::instance().getParameterName(kpa1), "", QMinuit::instance().getParameterName(kpa2));

    QMinuit::instance().setErrorDef(9);
    QMinuit::instance().contour(xcoor, ycoor, knpoints, kpa1, kpa2);
    pl.addGraph("3-σ", xcoor, ycoor);

    QMinuit::instance().setErrorDef(4);
    QMinuit::instance().contour(xcoor, ycoor, knpoints, kpa1, kpa2);
    pl.addGraph("2-σ", xcoor, ycoor);

    QMinuit::instance().setErrorDef(1);
    QMinuit::instance().contour(xcoor, ycoor, knpoints, kpa1, kpa2);
    pl.addGraph("1-σ", xcoor, ycoor);

    pl.draw();

}
