// Author: Spencer Wheaton 22 September 2009 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncBSQ(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 1));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 2));

    int check = model->primPartDens();

    if (!check) {

        double vol = model->getParameterSet()->getVolume();
        double y0 = ((PARAMETERSS *)p)->p1;
        double y1 = ((PARAMETERSS *)p)->p2;
        double y2 = ((PARAMETERSS *)p)->p3;

        double scale = 1.;

        if (y0 != 0.)
            gsl_vector_set(f, 0, (model->getBaryon() * vol - y0) / y0 * scale);
        else
            gsl_vector_set(f, 0, (model->getBaryon() * vol - y0) / ((qAbs(model->getBplus()) + qAbs(model->getBminus())) * vol) * scale);

        if (y1 != 0.)
            gsl_vector_set(f, 1, (model->getStrange() * vol - y1) / y1 * scale);
        else
            gsl_vector_set(f, 1, (model->getStrange() * vol - y1) / ((qAbs(model->getSplus()) + qAbs(model->getSminus())) * vol) * scale);

        if (y2 != 0.)
            gsl_vector_set(f, 2, (model->getCharge() * vol - y2) / y2 * scale);
        else
            gsl_vector_set(f, 2, (model->getCharge() * vol - y2) / ((qAbs(model->getQplus()) + qAbs(model->getQminus())) * vol) * scale);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
