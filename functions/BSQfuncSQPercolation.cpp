// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "external/particlesdbmanager.h"
#include "functions/FncsConstrain.h"

#include "main/TTMDensObj.h"
#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int  BSQfuncSQPercolation(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;

    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 2));

    double vh = 4. / 3. * M_PI * qPow(0.8, 3.);

    int check = model->primPartDens();

    if (!check) {
        model->generateEnergyDens();
        double p1 = ((PARAMETERSS *)p)->p1;
    if (p1 != 0.)
      gsl_vector_set(f, 0, (model->getStrange() - p1) / p1);
    else
      gsl_vector_set(f, 0, (model->getStrange() - p1) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));
    double p2 = ((PARAMETERSS *)p)->p2;
    gsl_vector_set(f, 1, (model->getBaryon() / 2. / model->getCharge() - p2) / p2);
    gsl_vector_set(f, 2, (model->getDensity() - 1.24 / vh * (1. - model->getBaryon() / model->getDensity()) - 0.34 / vh * model->getBaryon() / model->getDensity()) / (model->getDensity()));
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
  }
