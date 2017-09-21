// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017


#include "external/particlesdbmanager.h"

#include "main/TTMDensObj.h"
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>


extern TTMThermalModelBQ *gModelBQConQBDens;
extern double gBQyQBDens[1];

void BQfuncQBDens(int /*n*/, double x[], double f[])
{
    (gModelBQConQBDens->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
    (gModelBQConQBDens->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);

    double y[1];
    y[0]      = gModelBQConQBDens->getParameterSet()->getB2Q();
    int check = gModelBQConQBDens->primPartDens();

    if (!check) {
        double nb = 0.;
        for (TTMDensObj* dens : gModelBQConQBDens->getDensityTable()) {
            int id          = dens->getID();
            double partdens = dens->getPrimaryDensity();
            if(ParticlesDBManager::Instance().getBaryon(id) != 0.)
                nb += partdens;
        }

        f[1] = (nb - gBQyQBDens[0]) / gBQyQBDens[0];
        f[2] = (gModelBQConQBDens->getBaryon() / 2. / gModelBQConQBDens->getCharge() - y[0]) / y[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
