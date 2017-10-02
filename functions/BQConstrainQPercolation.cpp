// Author: Spencer Wheaton 14 July 2004 //
// Adaptated for Qt: Yves Schutz Septembre 2017

#include "functions/FncsConstrain.h"

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQConstrainQPercolation(TTMThermalModelBQ *model)
{    
    int rv = 0;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");

    const size_t ndim = 2;
    gsl_vector *x = gsl_vector_alloc(ndim);
    gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
    gsl_vector_set(x, 1, model->getParameterSet()->getMuQ());
    int  check = 0;
    PARAMETERS p;
    p.p0 = model;
    p.p1 = 0.0;
    broyden(x, ndim, check, p, BQfuncQPercolation);
    if (check) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(gsl_strerror(check));
        msg.exec();
        model->getParameterSet()->setConstraintInfo("Unable to Constrain to Percolation Model and B/2Q");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
        rv = 1;
    } else {
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
        model->getParameterSet()->setConstraintInfo("Percolation Model and B/2Q Successfully Constrained");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
    }
    gsl_vector_free(x);
    return rv;
}
