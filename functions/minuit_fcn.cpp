#include "main/TTMParameterSet.h"
#include "main/TTMThermalFit.h"
#include "main/TTMThermalModel.h"

#include <QDebug>

extern TTMThermalFit* gFit;
extern int            gFitParameters;        // number of fit parameters
extern double         gMinF;                 // minimum chi-square or quad-deviation
extern bool           gChi;                  // true if chi-square fit
extern bool           gQDev;                 // true if quad deviation fit

//__________________________________________________________________________
void Minuit_fcn(int & /*npar*/, double * /*gin*/, double & f, double * par, int /*iflag*/)
{

    // Change fit parameters //

    //  int imax = 0;
    //  QString fitDescr = gFit->getDescriptor();

    //  if(fitDescr == "GCanonical")
    //    imax = 9;
    //  else if(fitDescr == "SCanonical")
    //    imax = 5;
    //  else if(fitDescr == "BSQCanonical")
    //    imax = 5;

    int k = 0;
    for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
        if (current->getFlag() == 0) {
            current->setStatus("(** FITTING **)");
            current->setValue(par[k]);
            k++;
        }
    }
    // Calculate new densities and chi-square or quad-dev //

    gFit->generateYields();

    qInfo() <<"  *********************************** FITTING **********************************";

    // *** //

    for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
        qInfo() << current->objectName() << current->getValue() << current->getStatus();
    }
    qInfo() << (gFit->getParameterSet())->getConstraintInfo();
    qInfo() <<"  ******************************************************************************";
    // *** //

    qInfo() <<"  **********************";
    if (gChi) {
        f = gFit->getChiSquare();
        qInfo() << "ChiSquare = " << f;
    } else if (gQDev) {
        f = gFit->getQuadDev();
        qInfo() << "Quad Dev  = " << f;
    }
    qInfo() <<"  **********************";

    TTMThermalModel *model = gFit->generateThermalModel();
    model->generateParticleDens();
    qInfo() << "\t\t S/V \t  = \t" << model->getStrange();
    qInfo() << "\t\t B/2Q \t = \t" << model->getBaryon() / 2. / model->getCharge();
    qInfo() << "\t\t C/V \t  = \t" << model->getCharm();
    qInfo() << "\t\t b/V \t  = \t" << model->getBeauty();

    // Check for minimum and display //

    if (f < gMinF) {
        gMinF = f;
        qInfo() << "\t New Minimum!";
        for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
            TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
            qInfo() << current->objectName() << current->getValue() << current->getStatus();
        }
        qInfo() << (gFit->getParameterSet())->getConstraintInfo();
        qInfo() <<"  ******************************************************************************";
        gFit->listYields();
    } else {
        qInfo() <<"  ******************************************************************************";
    }
}
