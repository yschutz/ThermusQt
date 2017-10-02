// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "functions/FncsConstrain.h"

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQConstrainBDens(TTMThermalModelBQ *model, double nb)
{    
    int rv = 0;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    if (nb == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain nb to zero");
        msg.exec();
        rv = 1;
    } else {
        const size_t ndim = 1;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        int  check = 0;
        PARAMETERS p;
        p.p0 = model;
        p.p1 = nb;
        broyden(x, ndim, check, p, BQfuncBDens);
        if(check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain nb");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            rv = check;
        } else {        
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("nb Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
