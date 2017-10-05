// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncSQC(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;

    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUC)->setValue(gsl_vector_get(x, 2));

    bool check = model->primPartDens();

    if (check) {
        double y0 = model->getParameterSet()->getDens(TTMParameterSet::kMUS);
        double y1 = model->getParameterSet()->getB2Q();
        double y2 = model->getParameterSet()->getDens(TTMParameterSet::kMUC);

        if (y0 != 0.)
            gsl_vector_set(f, 0, (model->getStrange() - y0) / y0);
        else
            gsl_vector_set(f, 0, (model->getStrange() - y0) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));

        gsl_vector_set(f, 1, (model->getBaryon() / 2. / model->getCharge() - y1) / y1);

        if( y2 != 0.)
            gsl_vector_set(f, 2, (model->getCharm() - y2) / y2);
        else
            gsl_vector_set(f, 2, (model->getCharm() - y2) / (qAbs(model->getCplus()) + qAbs(model->getCminus())));
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
