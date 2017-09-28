// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConQEN;
extern double gBQyQEN[1];

//__________________________________________________________________________
int BQfuncQEN(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConQEN->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (gModelBQConQEN->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));

    double y = gModelBQConQEN->getParameterSet()->getB2Q();
    bool check = gModelBQConQEN->primPartDens();

    if (check) {
        gModelBQConQEN->generateEnergyDens();
        gsl_vector_set(f, 0, (gModelBQConQEN->getEnergy() / gModelBQConQEN->getDensity() - gBQyQEN[0]) /gBQyQEN[0]);
        gsl_vector_set(f, 1, (gModelBQConQEN->getBaryon() / 2. /gModelBQConQEN->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
