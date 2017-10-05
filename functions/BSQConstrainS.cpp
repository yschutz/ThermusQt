// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"
#include <QMessageBox>

//__________________________________________________________________________
int BSQConstrainS(TTMThermalModelBSQ *model)
{
    model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("");
    int  rv = 0;

    const size_t ndim = 1;
    gsl_vector *x = gsl_vector_alloc(ndim);
    gsl_vector_set(x, 0, model->getParameterSet()->getMuS());
    int check = 0;
    PARAMETERSS p;
    p.p0 = model;
    p.p1 = 0.0;
    p.p2 = 0.0;
    p.p3 = 0.0;
    broyden(x, ndim, check, p, BSQfuncS);
    if (check) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(gsl_strerror(check));
        msg.exec();
        model->getParameterSet()->setConstraintInfo("Unable to Constrain S/V");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(Unable to constrain)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(0.);
        rv = check;
    } else {
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
        model->getParameterSet()->setConstraintInfo("S/V Successfully Constrained");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(*CONSTRAINED*)");
    }
    gsl_vector_free(x);
    return rv;
}
