// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz September 2017

#include "FncsConstrain.h"

#include "TTMParameterSet.h"
#include "TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQConstrainEN(TTMThermalModelBQ *model, double eovern)
{    
    int rv = 0;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    if (eovern == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot conserve E/N to zero");
        msg.exec();
        return 1;
    } else {
        const size_t ndim = 1;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        int  check = 0;
        PARAMETERS p;
        p.p0 = model;
        p.p1 = eovern;
        broyden(x, ndim, check, p, BQfuncEN);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain E/N");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            rv = 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("E/N Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
