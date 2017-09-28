// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "external/particlesdbmanager.h"

#include "main/TTMDensObj.h"
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>


extern TTMThermalModelBQ *gModelBQConQBDens;
extern double gBQyQBDens[1];

//__________________________________________________________________________
void BQfuncQBDens(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    (gModelBQConQBDens->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (gModelBQConQBDens->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));

    double y = gModelBQConQBDens->getParameterSet()->getB2Q();
    bool check = gModelBQConQBDens->primPartDens();

    if (check) {
        double nb = 0.;
        for (TTMDensObj* dens : gModelBQConQBDens->getDensityTable()) {
            int id          = dens->getID();
            double partdens = dens->getPrimaryDensity();
            if(ParticlesDBManager::Instance().getBaryon(id) != 0.)
                nb += partdens;
        }

        gsl_vector_set(f, 0, (nb - gBQyQBDens[0]) / gBQyQBDens[0]);
        gsl_vector_set(f, 1, (gModelBQConQBDens->getBaryon() / 2. / gModelBQConQBDens->getCharge() - y) / y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
}
