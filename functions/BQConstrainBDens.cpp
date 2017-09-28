// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

void broyden(gsl_vector* x, size_t n, int& status, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));
int BQfuncBDens(const gsl_vector* x, void* p, gsl_vector* f);

TTMThermalModelBQ *gModelBQConBDens;
double gBQyBDens[1];

//__________________________________________________________________________
int BQConstrainBDens(TTMThermalModelBQ *model, double nb)
{    
    gModelBQConBDens = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    gBQyBDens[0] = nb;

    if (gBQyBDens[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain nb to zero");
        msg.exec();
        return 1;
    } else {
        const size_t ndim = 1;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        int  check = 0;
        broyden(x, ndim, check, BQfuncBDens);
        if(check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain nb");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            return check;
        } else {        
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->setConstraintInfo("nb Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
