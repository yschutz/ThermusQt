// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "external/particlesdbmanager.h"

#include "main/TTMDensObj.h"
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConBDens;
extern double gBQyBDens[1];

void BQfuncBDens(int n, double x[], double f[])
{
    (gModelBQConBDens->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);

    int check = gModelBQConBDens->primPartDens();

    if (!check) {
        double nb = 0.;
        for (TTMDensObj* dens : gModelBQConBDens->getDensityTable()) {
            int id          = dens->getID();
            double partdens = dens->getPrimaryDensity();

            if(ParticlesDBManager::Instance().getBaryon(id) != 0.)
                nb += partdens;
        }

        f[1] = (nb - gBQyBDens[0]) / gBQyBDens[0];

    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
