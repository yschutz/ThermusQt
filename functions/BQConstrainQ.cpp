// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz September 2017

#include "functions/FncsConstrain.h"

#include "main/TTMParameterSet.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQConstrainQ(TTMThermalModelBQ *model)
{
    int rv = 0;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    if(model->getParameterSet()->getB2Q() == 0.){
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain B/2Q to zero");
        msg.exec();
        return 1;
    } else {
        const size_t ndim = 1;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->getValue());
        int  check = 0;
        PARAMETERS p;
        p.p0 = model;
        p.p1 = 0.0;
        broyden(x, ndim, check, p, BQfuncQ);
        if(check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            rv = 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
