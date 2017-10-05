// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncSEN(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;

    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));

    bool check = model->primPartDens();

    if (check) {
        model->generateEnergyDens();
        double ene1 = ((PARAMETERSS *)p)->p1;
        if ( ene1 != 0.)
            gsl_vector_set(f, 0, (model->getStrange() - ene1) / ene1);
        else
            gsl_vector_set(f, 0, (model->getStrange() - ene1)/(qAbs(model->getSplus()) + qAbs(model->getSminus())));

        double ene2 = ((PARAMETERSS *)p)->p2;
        gsl_vector_set(f, 1, (model->getEnergy() / model->getDensity() - ene2) / ene2);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
