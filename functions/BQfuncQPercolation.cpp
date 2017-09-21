// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

extern TTMThermalModelBQ *gModelBQConQPercolation;

void BQfuncQPercolation(int n, double x[], double f[])
{
    (gModelBQConQPercolation->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
    (gModelBQConQPercolation->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);

    double y[1];
    y[0]      = gModelBQConQPercolation->getParameterSet()->getB2Q();
    double vh = 4./3.* M_PI * qPow(0.8, 3.);
    int check = gModelBQConQPercolation->primPartDens();

    if (!check) {
        gModelBQConQPercolation->generateEnergyDens();
        f[1] = (gModelBQConQPercolation->getDensity() - 1.24 / vh *
                (1. - gModelBQConQPercolation->getBaryon() / gModelBQConQPercolation->getDensity()) - 0.34 /
                vh * gModelBQConQPercolation->getBaryon() / gModelBQConQPercolation->getDensity()) /
                (gModelBQConQPercolation->getDensity());
        f[2] = (gModelBQConQPercolation->getBaryon() / 2. / gModelBQConQPercolation->getCharge() - y[0]) / y[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
