// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "FncsConstrain.h"

#include "TTMParameterSetBQ.h"
#include "TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQfuncQNetBDens(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBQ* model = ((PARAMETERS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));

    double y = model->getParameterSet()->getB2Q();

    bool check = model->primPartDens();

    if (check) {
        double nb = model->getBaryon();
        double dens = ((PARAMETERS*)p)->p1;
        gsl_vector_set(f, 0, (nb - dens) / dens);
        gsl_vector_set(f, 1, (model->getBaryon() /2. / model->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
