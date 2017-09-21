// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConEN;
extern double gBQyEN[1];

void BQfuncEN(int n, double x[], double f[])
{
    (gModelBQConEN->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);

    int check = gModelBQConEN->primPartDens();

    if (!check) {
        gModelBQConEN->generateEnergyDens();
        f[1] = (gModelBQConEN->getEnergy() / gModelBQConEN->getDensity() - gBQyEN[0]) / gBQyEN[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
