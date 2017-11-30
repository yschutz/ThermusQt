// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Noveber 2017

#include "external/QMinuit.h"
#include "main/TTMParameterSet.h"
#include "main/TTMThermalFit.h"
#include "mainwindow.h"

#include <QDebug>

TTMThermalFit* gFit;
int            gFitParameters;        // number of fit parameters
double         gMinF = 10000000.;     // minimum chi-square or quad-deviation
bool           gChi;                  // true if chi-square fit
bool           gQDev;                 // true if quad deviation fit

void Minuit_fcn(int & npar, double * gin, double & f, double * par, int iflag);

//__________________________________________________________________________
void fit_function(TTMThermalFit * fit, int flag = 0)
{
    gFit     = fit;

    if (flag == 0) {
        gChi  = true;
        gQDev = false;
    } else if (flag == 1) {
        gChi  = false;
        gQDev = true;
    }

    // Check # of fit parameters //

    gFitParameters = 0;

    for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
        if (current->getFlag() == 0)
            gFitParameters++;
    }
    QMinuit::instance().setMaxParameters(gFitParameters);
    QMinuit::instance().setFCN(Minuit_fcn);
    QMinuit::instance().setErrorDef(1.);  		// 1-sigma errors

    // Set fit parameters //

    int k          = 0;
    int error_flag = 0;

    for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
        if (current->getFlag() == 0) {
            QMinuit::instance().qmNParm(k, current->objectName(),
                                        current->getStart(), current->getStep(),
                                        current->getMin(), current->getMax(),
                                        error_flag); // error_flag = 0 if no problems
            k++;
        }
    }
    // List Experimental Yields //

    gFit->listYields(true);

    // ************************************************ //
    // ************************************************ //

    // arglist[0] : max # of iterations
    // arglist[1] : convergence when EDM < arglist[1] * 10^-3
    // check error_flag afterwards to see if command executed normally

    // Other options:
    //
    // double temp = 1000;
    // lMinuit->mnexcm("MIGRAD", &temp, 1, error_flag);
    // (sets max iterations to 1000)
    //            OR
    // lMinuit->mnexcm("MIGRAD", &temp, 0, error_flag);
    // (uses defaults)

    double arglist[10] = {0.0};
    arglist[0] = 15000;
    arglist[1] = 1;

    QMinuit::instance().qmnexcm("MIGRAD", arglist, 2, error_flag);
    //     // ************************************************ //

    //      // Return the current status of the minimization
    //      // Check icstat for state of covariance matrix

    //      double amin, edm, errdef;
    //      int nvpar, nparx, icstat;
    //      lMinuit->qmnstat(amin, edm, errdef, nvpar, nparx, icstat);

    //      // ************************************************ //

    //      // Outputs the results depending on the first flag

    //      lMinuit->qmnprin(1, amin);

    //      // ************************************************ //
    //      // ************************************************ //

    //      // Now after fit update parameter set with best-fit //
    //      // values, re-calculate densities and display       //

    //      int kk = 0;

    //      for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
    //        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
    //        if (current->getFlag() == 0) {
    //          double val, error;
    //          lMinuit->getParameter(kk, val, error);
    //          current->setValue(val);
    //          current->setError(error);
    //          current->setStatus("(FITTED!)");
    //          kk++;
    //        }
    //      }

    //      gFit->generateYields();
    //      gFit->setMinuit(lMinuit);
}
