// Author: Spencer Wheaton 22 September 2009 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQConstrainSQNetBDens(TTMThermalModelBSQ *model, double nb)
{    
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    int  rv = 0;

    if (nb == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot conserve (nb+nbbar) to zero");
        msg.exec();
        rv = 1;
    } else {
        const size_t ndim = 3;
        gsl_vector *x = gsl_vector_alloc(ndim);

        gsl_vector_set(x, 0, model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->getValue());
        gsl_vector_set(x, 1, model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->getValue());
        gsl_vector_set(x, 2, model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->getValue());

        int check = 0;
        PARAMETERSS p;
        p.p0 = model;
        p.p1 = model->getParameterSet()->getDens(TTMParameterSet::kMUS);
        p.p2 = nb;
        p.p3 = model->getParameterSet()->getB2Q();

        broyden(x, ndim, check, p, BSQfuncSQNetBDens);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/V, B/2Q & (nb-nbbar)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            rv = 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 2));
            model->getParameterSet()->setConstraintInfo("S/V, B/2Q & (nb-nbbar) Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
