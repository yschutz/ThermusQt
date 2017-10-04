// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncQ(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 0));
    bool check = model->primPartDens();

    double xx = gsl_vector_get(x, 0);
    while (check) {
        xx = xx * 0.9;
        (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(xx);
        check = model->primPartDens();
    }
    double b2q = model->getParameterSet()->getB2Q();
    gsl_vector_set(f, 0, (model->getBaryon() / 2. / model->getCharge() - b2q) / b2q);

    return rv;
}
