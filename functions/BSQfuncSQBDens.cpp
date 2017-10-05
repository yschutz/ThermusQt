// Author: Spencer Wheaton 24 November 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "external/particlesdbmanager.h"
#include "functions/FncsConstrain.h"

#include "main/TTMDensObj.h"
#include "main/TTMThermalModelBSQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BSQfuncSQBDens(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBSQ* model = ((PARAMETERSS *)p)->p0;

    (model->getParameterSet())->getParameter(TTMParameterSet::kMUS)->setValue(gsl_vector_get(x, 0));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 1));
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 2));

    bool check = model->primPartDens();

    if (check) {
        double sq0 = ((PARAMETERSS *)p)->p1;


        if (sq0 != 0.)
            gsl_vector_set(f, 0, (model->getStrange() - sq0) / sq0);
        else
            gsl_vector_set(f, 0, (model->getStrange() - sq0) / (qAbs(model->getSplus()) + qAbs(model->getSminus())));

        QHashIterator<int, TTMDensObj*> next(model->getDensityTable());
        double nb = 0.;
        while(next.hasNext()) {
            int id           = next.key();
            TTMDensObj *dens = next.value();
            double partdens  = dens->getPrimaryDensity();

            if(ParticlesDBManager::Instance().getBaryon(id) != 0.)
                nb += partdens;
        }
        double sqb1 = ((PARAMETERSS *)p)->p2;
        gsl_vector_set(f, 1, (nb - sqb1) / sqb1);

        double sqb2 = ((PARAMETERSS *)p)->p2;
        gsl_vector_set(f, 2, (model->getBaryon()/ 2. / model->getCharge() - sqb2) / sqb2);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
