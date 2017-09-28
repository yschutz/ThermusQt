// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

void broyden(gsl_vector* x, size_t n, int& status, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));
int BQfuncQNetBDens(const gsl_vector* x, void* p, gsl_vector* f);

TTMThermalModelBQ *gModelBQConQNetBDens;
double gBQyQNetBDens[1];

//__________________________________________________________________________
int BQConstrainQNetBDens(TTMThermalModelBQ *model, double nb)
{    

    gModelBQConQNetBDens = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    gBQyQNetBDens[0] = nb;

    if (gBQyQNetBDens[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain nb to zero");
        msg.exec();
        return 1;
    } else if (model->getParameterSet()->getB2Q() == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Cannot constrain B/2Q to zero");
        msg.exec();
        return 1;
    } else {
        const size_t ndim = 2;
        gsl_vector *x = gsl_vector_alloc(ndim);
        gsl_vector_set(x, 0, model->getParameterSet()->getMuB());
        gsl_vector_set(x, 1, model->getParameterSet()->getMuQ());
        int  check = 0;
        broyden(x, ndim, check, BQfuncQNetBDens);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
            msg.setInformativeText(gsl_strerror(check));
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain nb and B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
            model->getParameterSet()->setConstraintInfo("nb and B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
