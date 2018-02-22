// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "FncsConstrain.h"

#include "TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncS(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));

    int check = model->primPartDens();

    if (!check) {
        double densS = model->getParameterSet()->getDens(TTMParameterSet::kMUS);
        if (densS != 0.)
            gsl_vector_set(f, 0, (model->getStrange() - densS) / densS);
        else
            gsl_vector_set(f, 0, (model->getStrange() - densS) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
