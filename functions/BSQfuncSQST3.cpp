// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncSQST3(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;

    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 2));

    int check = model->primPartDens();

    if (!check) {
        model->generateEntropyDens();

        double ss0 = ((PARAMETERSS *)p)->p1;

        if (ss0 != 0.)
            gsl_vector_set(f, 0, (model->getStrange() - ss0) / ss0);
        else
            gsl_vector_set(f, 0, (model->getStrange() - ss0 )/ (qAbs(model->getSplus()) + qAbs(model->getSminus())));

        double ss1 = ((PARAMETERSS *)p)->p2;
        gsl_vector_set(f, 1, (model->getBaryon()/2./ model->getCharge() - ss1) / ss1);

        double ss2 = ((PARAMETERSS *)p)->p3;
        gsl_vector_set(f, 2, (model->getEntropy()/ qPow(model->getParameterSet()->getT(), 3.) * qPow(0.197, 3.) - ss2) / ss2);

    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
