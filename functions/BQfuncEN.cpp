// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConEN;
extern double gBQyEN[1];

int BQfuncEN(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConEN->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));

    bool check = gModelBQConEN->primPartDens();

    if (check) {
        gModelBQConEN->generateEnergyDens();
        gsl_vector_set(f, 0, (gModelBQConEN->getEnergy() / gModelBQConEN->getDensity() - gBQyEN[0]) / gBQyEN[0]);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
