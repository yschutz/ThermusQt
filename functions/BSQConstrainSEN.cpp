// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQConstrainSEN(TTMThermalModelBSQ *model, double eovern)
{    
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("");
    int  rv = 0;

    if (eovern == 0.0) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot conserve E/N to zero");
        msg.exec();
        rv = 1;
    } else {
        const size_t ndim = 2;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuS());
        gsl_vector_set(x, 1, model->getParameterSet()->getMuB());
        int check = 0;
        PARAMETERSS p;
        p.p0 = model;
        p.p1 = model->getParameterSet()->getDens(TTMParameterSet::kMUS);
        p.p2 = eovern;
        p.p3 = 0.0;
        broyden(x, ndim, check, p, BSQfuncSEN);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/V & E/N");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(0.);
            rv = check;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));
            model->getParameterSet()->setConstraintInfo("S/V & E/N Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}
