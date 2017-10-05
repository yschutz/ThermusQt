// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQConstrainSQC(TTMThermalModelBSQ *model)
{
    model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUC)->setStatus("");
    int  rv = 0;


    if (model->getParameterSet()->getB2Q() == 0.){
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain B/2Q to zero");
        msg.exec();
        rv = 1;
    } else {
        const size_t ndim = 3;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->getValue());
        gsl_vector_set(x, 1, model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->getValue());
        gsl_vector_set(x, 2, model->getParameterSet()->getParameter(TTMParameterSet::kMUC)->getValue());
        int check = 0;
        PARAMETERSS p;
        p.p0 = model;
        p.p1 = 0.0;
        p.p2 = 0.0;
        p.p3 = 0.0;
        broyden(x, ndim, check, p, BSQfuncSQC);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/V, B/2Q and C/V");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUC)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUC)->setValue(0.);
            rv = check;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUC)->setValue(gsl_vector_get(x, 2));
            model->getParameterSet()->setConstraintInfo("S/V, B/2Q and C/V Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUS)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUC)->setStatus("(*CONSTRAINED*)");
        }
        gsl_vector_free(x);
    }
    return rv;
}

