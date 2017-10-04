// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncS(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    bool check = model->primPartDens();

    double xx = gsl_vector_get(x, 0);
    while (check) {
        xx = xx * 0.9;
        (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(xx);
        check = model->primPartDens();

    }
    double densS = model->getParameterSet()->getDens(TTMParameterSet::kMUS);
    if (densS != 0.)
        gsl_vector_set(f, 0, (model->getStrange() - densS) / densS);
    else
        gsl_vector_set(f, 0, (model->getStrange() - densS) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));

    return rv;
}
