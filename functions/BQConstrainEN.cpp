// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz September 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSet.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

void broyden(gsl_vector* x, size_t n, int& status, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));
int BQfuncEN(const gsl_vector* x, void* p, gsl_vector* f);

TTMThermalModelBQ *gModelBQConEN;
double gBQyEN[1];

//__________________________________________________________________________
int BQConstrainEN(TTMThermalModelBQ *model, double eovern)
{    
    gModelBQConEN = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    gBQyEN[0] = eovern;

    if (gBQyEN[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot conserve E/N to zero");
        msg.exec();
        return 1;
    } else {
        const size_t ndim = 1;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        int  check = 0;
        broyden(x, ndim, check, BQfuncEN);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain E/N");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("E/N Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
