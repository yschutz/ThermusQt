// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConQNetBDens;
extern double gBQyQNetBDens[1];

//__________________________________________________________________________
int BQfuncQNetBDens(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConQNetBDens->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (gModelBQConQNetBDens->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));

    double y = gModelBQConQNetBDens->getParameterSet()->getB2Q();

    bool check = gModelBQConQNetBDens->primPartDens();

    if (check) {
        double nb = gModelBQConQNetBDens->getBaryon();
        gsl_vector_set(f, 0, (nb - gBQyQNetBDens[0]) / gBQyQNetBDens[0]);
        gsl_vector_set(f, 1, (gModelBQConQNetBDens->getBaryon() /2. / gModelBQConQNetBDens->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
