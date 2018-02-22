// Author: Spencer Wheaton 22 September 2009 //
// Adapted for Qt by Yves Schutz: October 2017

#include "FncsConstrain.h"

#include "TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int  BSQConstrainSPercolation(TTMThermalModelBSQ *model)
{
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("");
    int  rv = 0;

    const size_t ndim = 2;
    gsl_vector *x = gsl_vector_alloc(ndim);

    gsl_vector_set(x, 0, model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->getValue());
    gsl_vector_set(x, 1, model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->getValue());

    int check = 0;
    PARAMETERSS p;
    p.p0 = model;
    p.p1 = model->getParameterSet()->getDens(TTMParameterSet::kMUS);
    p.p2 = 0.0;
    p.p3 = 0.0;

    broyden(x, ndim, check, p, BSQfuncSPercolation);
    if (check) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(gsl_strerror(check));
        msg.exec();
        model->getParameterSet()->setConstraintInfo("Unable to Constrain S/V together with Percolation Model");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(Unable to constrain)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(0.);
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
        rv = 1;
    } else {
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));
        model->getParameterSet()->setConstraintInfo("S/V Successfully Constrained together with Percolation Model");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(*CONSTRAINED*)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
    }
    gsl_vector_free(x);
    return rv;
}
