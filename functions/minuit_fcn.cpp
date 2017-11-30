#include "main/TTMParameterSet.h"
#include "main/TTMThermalFit.h"
#include "main/TTMThermalModel.h"
#include "Logger.h"

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

    QString pr("  *********************************** FITTING **********************************");
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());

    // *** //

    for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
        pr = QString("%1 = %2 %3").arg(current->objectName(), 20).
                arg(current->getValue(), 10, 'e', 5).
                arg(current->getStatus(), 20);
        Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    }
    pr = QString("%1").arg(gFit->getParameterSet()->getConstraintInfo(), 40);
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    pr = "  ******************************************************************************";
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    // *** //

    pr = "********************** ";
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    if (gChi) {
        f = gFit->getChiSquare();
        pr = pr + QString("ChiSquare = %1").arg(f, 10, 'g', 7);
    } else if (gQDev) {
        f = gFit->getQuadDev();
        pr = pr + QString("Quad Dev  = %1").arg(f, 10, 'g', 7);
    }
    pr = pr + " **********************";
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());

    TTMThermalModel *model = gFit->generateThermalModel();
    model->generateParticleDens();
    pr = QString("     S/V  = %1").arg(model->getStrange(), 12, 'e', 6);
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    pr = QString("    B/2Q  = %1").arg(model->getBaryon() / 2. / model->getCharge(), 12, 'e', 6);
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    pr = QString("     C/V  = %1").arg(model->getCharm(), 12, 'e', 6);
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    pr = QString("     b/V  = %1").arg(model->getBeauty(), 12, 'e', 6);
    Logger::instance().writeMessage(pr, Logger::instance().isVerbose());

    // Check for minimum and display //

    if (f < gMinF) {
        gMinF = f;
        pr = "\t New Minimum!";
        Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
        for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
            TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
            pr = QString("%1 = %2 %3").arg(current->objectName(), 20).
                    arg(current->getValue(), 10, 'e', 5).
                    arg(current->getStatus(), 20);
            Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
        }
        pr = QString("%1").arg(gFit->getParameterSet()->getConstraintInfo(), 40);
        Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
        pr = "  ******************************************************************************";
        Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
        if (Logger::instance().isVerbose())
            gFit->listYields(true);
    } else {
        pr = "  ******************************************************************************";
        Logger::instance().writeMessage(pr, Logger::instance().isVerbose());
    }
}
