// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "FncsConstrain.h"

#include "TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncQQ(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 0));

    double vol = (model->getParameterSet())->getVolume();

    int check = model->primPartDens();

    if (!check) {

        double qq = ((PARAMETERSS *)p)->p1;

        if (qq != 0.)
            gsl_vector_set(f, 0, (model->getCharge() * vol - qq) / qq);
        else
            gsl_vector_set(f, 0, (model->getCharge() * vol - qq) / (vol * qAbs(model->getQplus()) + qAbs(model->getQminus())));

    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
