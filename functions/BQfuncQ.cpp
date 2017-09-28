// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConQ;

int BQfuncQ(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConQ->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 0));
    bool check = gModelBQConQ->primPartDens();

    if (check) {
        double y = gModelBQConQ->getParameterSet()->getB2Q();
        gsl_vector_set(f, 0, (gModelBQConQ->getBaryon() / 2. / gModelBQConQ->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Problems with parameters");
        msg.exec();
        exit(1);
    }
    return rv;
}
