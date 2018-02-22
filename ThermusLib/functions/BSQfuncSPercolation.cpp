// Author: Spencer Wheaton 24 November 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "particlesdbmanager.h"
#include "FncsConstrain.h"

#include "TTMDensObj.h"
#include "TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncSPercolation(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;

  (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
  (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));

  double vh = 4. / 3. * M_PI * qPow(0.8, 3.);

  int check = model->primPartDens();

  if (!check) {
    model->generateEnergyDens();
    double p1 = ((PARAMETERSS *)p)->p1;
    if (p1 != 0.)
      gsl_vector_set(f, 0, (model->getStrange() - p1) / p1);
    else
      gsl_vector_set(f, 0, (model->getStrange() - p1) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));

    gsl_vector_set(f, 1, (model->getDensity() - 1.24 / vh * (1. - model->getBaryon() / model->getDensity()) - 0.34 / vh * model->getBaryon() / model->getDensity()) / (model->getDensity()));
  } else {
      QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
      msg.setInformativeText("Primary particles density problems!");
      msg.exec();
      exit(1);
  }
  return rv;
}
