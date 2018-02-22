// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "FncsConstrain.h"

#include "TTMParameterSetBQ.h"
#include "TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQfuncQ(const gsl_vector *x, void *p, gsl_vector *f)
{
    int rv = 0;
    TTMThermalModelBQ* model = ((PARAMETERS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 0));
    bool check = model->primPartDens();

    if (check) {
        double y = model->getParameterSet()->getB2Q();
        gsl_vector_set(f, 0, (model->getBaryon() / 2. / model->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Problems with parameters");
        msg.exec();
        exit(1);
    }
    return rv;
}
