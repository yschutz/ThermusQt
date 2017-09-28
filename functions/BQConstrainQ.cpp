// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz September 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSet.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

void broyden(gsl_vector* x, size_t n, int& status, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));
int BQfuncQ(const gsl_vector* x, void* p, gsl_vector* f);

TTMThermalModelBQ *gModelBQConQ;

//__________________________________________________________________________
int BQConstrainQ(TTMThermalModelBQ *model)
{
    gModelBQConQ = model;

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
        broyden(x, ndim, check, BQfuncQ);

        if(check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
