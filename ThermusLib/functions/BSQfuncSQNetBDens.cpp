// Adapted for Qt by Yves Schutz: October 2017

#include "FncsConstrain.h"

#include "TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncSQNetBDens(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 2));

    int check = model->primPartDens();
    if (!check) {
        double p1 = ((PARAMETERSS *)p)->p1;
        if (p1 != 0.)
            gsl_vector_set(f, 0, (model->getStrange() - p1) / p1);
        else
            gsl_vector_set(f, 0, (model->getStrange() - p1) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));

        double nb = model->getBaryon();
        double p2 = ((PARAMETERSS *)p)->p2;
        gsl_vector_set(f, 1, (nb - p2) / p2);

        double p3 = ((PARAMETERSS *)p)->p3;
        gsl_vector_set(f, 2, (model->getBaryon() / 2. / model->getCharge() - p3) / p3);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
