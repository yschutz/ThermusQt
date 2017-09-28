// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "external/particlesdbmanager.h"

#include "main/TTMDensObj.h"
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConBDens;
extern double gBQyBDens[1];

//__________________________________________________________________________
int BQfuncBDens(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConBDens->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));

    bool check = gModelBQConBDens->primPartDens();

    if (check) {
        double nb = 0.;
        for (TTMDensObj* dens : gModelBQConBDens->getDensityTable()) {
            int id          = dens->getID();
            double partdens = dens->getPrimaryDensity();

            if(ParticlesDBManager::Instance().getBaryon(id) != 0.)
                nb += partdens;
        }
        gsl_vector_set(f, 0, (nb - gBQyBDens[0]) / gBQyBDens[0]);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
