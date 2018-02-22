// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "FncsConstrain.h"

#include "TTMParameterSetBQ.h"
#include "TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQConstrainST3(TTMThermalModelBQ *model, double soverT3)
{    

    int rv = 0;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    if (soverT3 == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain S/T^3 to zero");
        msg.exec();
        rv = 1;
    } else {
        const size_t ndim = 1;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        int  check = 0;
        PARAMETERS p;
        p.p0 = model;
        p.p1 = soverT3;
        broyden(x, ndim, check, p, BQfuncST3);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/T^3");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            rv = 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("S/T^3 Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
