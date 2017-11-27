// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "external/particlesdbmanager.h"

#include "functions/FncsConstrain.h"

#include "main/TTMDensObj.h"
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//__________________________________________________________________________
int BQfuncBDens(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBQ* model = ((PARAMETERS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));

    bool check = model->primPartDens();

    if (check) {
        double nb = 0.;
        for (TTMDensObj* dens : model->getDensityTable()) {
            int id          = dens->getID();
            double partdens = dens->getPrimaryDensity();

            if(ParticlesDBManager::instance().getBaryon(id) != 0.)
                nb += partdens;
        }
        double dens = ((PARAMETERS*)p)->p1;
        gsl_vector_set(f, 0, (nb - dens) / dens);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
