#include "main/TTMParameterSet.h"
#include "main/TTMThermalFit.h"
#include "main/TTMThermalModel.h"
#include "mainwindow.h"

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

    MainWindow::verbosePrint("  *********************************** FITTING **********************************");

    // *** //

    for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
        TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
        QString pr = QString("%1 = %2 %3").arg(current->objectName(), 20).
                arg(current->getValue(), 10, 'e', 5).
                arg(current->getStatus(), 20);
        MainWindow::verbosePrint(pr);
    }
    MainWindow::verbosePrint(QString("%1").arg(gFit->getParameterSet()->getConstraintInfo(), 40));
    MainWindow::verbosePrint("  ******************************************************************************");
    // *** //

    QString pr("********************** ");
    if (gChi) {
        f = gFit->getChiSquare();
        pr = pr + QString("ChiSquare = %1").arg(f, 10, 'g', 7);
    } else if (gQDev) {
        f = gFit->getQuadDev();
        pr = pr + QString("Quad Dev  = %1").arg(f, 10, 'g', 7);
    }
    pr = pr + " **********************";
    MainWindow::verbosePrint(pr);

    TTMThermalModel *model = gFit->generateThermalModel();
    model->generateParticleDens();
    //    if (MainWindow::isVerbose()) {
    MainWindow::verbosePrint(QString("     S/V  = %1").arg(model->getStrange(), 12, 'e', 6));
    MainWindow::verbosePrint(QString("    B/2Q  = %1").arg(model->getBaryon() / 2. / model->getCharge(), 12, 'e', 6));
    MainWindow::verbosePrint(QString("     C/V  = %1").arg(model->getCharm(), 12, 'e', 6));
    MainWindow::verbosePrint(QString("     b/V  = %1").arg(model->getBeauty(), 12, 'e', 6));
    //    }
    // Check for minimum and display //

    if (f < gMinF) {
        gMinF = f;
        MainWindow::verbosePrint("\t New Minimum!");
        for (int i = TTMParameterSet::kT; i < TTMParameterSet::kPARTYPES; i++) {
            TTMParameter* current = (gFit->getParameterSet())->getParameter(static_cast<TTMParameterSet::ParameterType>(i));
            QString pr = QString("%1 = %2 %3").arg(current->objectName(), 20).
                    arg(current->getValue(), 10, 'e', 5).
                    arg(current->getStatus(), 20);
            MainWindow::verbosePrint(pr);
        }
        MainWindow::verbosePrint(QString("%1").arg(gFit->getParameterSet()->getConstraintInfo(), 40));
        MainWindow::verbosePrint("  ******************************************************************************");
        if (MainWindow::isVerbose())
            gFit->listYields(true);
    } else
        MainWindow::verbosePrint("  ******************************************************************************");
}
