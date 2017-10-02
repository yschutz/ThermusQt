// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "functions/FncsConstrain.h"

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQConstrainQST3(TTMThermalModelBQ *model, double SoverT3)
{    
    int rv = 0;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    if (SoverT3 == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain S/T^3 to zero");
        msg.exec();
        rv = 1;
    } else if (model->getParameterSet()->getB2Q() == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain B/2Q to zero");
        msg.exec();
        rv = 1;
    } else {
        const size_t ndim = 2;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        gsl_vector_set(x, 1, model->getParameterSet()->getMuQ());
        int  check = 0;
        PARAMETERS p;
        p.p0 = model;
        p.p1 = SoverT3;
        broyden(x, ndim, check, p, BQfuncQST3);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/T^3 and B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            rv = check;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
            model->getParameterSet()->setConstraintInfo("S/T^3 and B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
