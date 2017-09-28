// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

extern TTMThermalModelBQ *gModelBQConQPercolation;

//__________________________________________________________________________
int BQfuncQPercolation(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConQPercolation->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (gModelBQConQPercolation->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));

    double y= gModelBQConQPercolation->getParameterSet()->getB2Q();
    double vh = 4./3.* M_PI * qPow(0.8, 3.);
    bool check = gModelBQConQPercolation->primPartDens();

    if (check) {
        gModelBQConQPercolation->generateEnergyDens();
        gsl_vector_set(f, 0, (gModelBQConQPercolation->getDensity() - 1.24 / vh *
                              (1. - gModelBQConQPercolation->getBaryon() / gModelBQConQPercolation->getDensity()) - 0.34 /
                              vh * gModelBQConQPercolation->getBaryon() / gModelBQConQPercolation->getDensity()) /
                       (gModelBQConQPercolation->getDensity()));
        gsl_vector_set(f, 1, (gModelBQConQPercolation->getBaryon() / 2. / gModelBQConQPercolation->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
