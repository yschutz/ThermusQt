// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConQEN;
extern double gBQyQEN[1];

void BQfuncQEN(int n, double x[], double f[])
{
    (gModelBQConQEN->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
    (gModelBQConQEN->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);

    double y[1];

    y[0]      = gModelBQConQEN->getParameterSet()->getB2Q();
    int check = gModelBQConQEN->primPartDens();

    if (!check) {
        gModelBQConQEN->generateEnergyDens();
        f[1] = (gModelBQConQEN->getEnergy() / gModelBQConQEN->getDensity() - gBQyQEN[0]) /gBQyQEN[0];
        f[2] = (gModelBQConQEN->getBaryon() / 2. /gModelBQConQEN->getCharge() - y[0]) / y[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
