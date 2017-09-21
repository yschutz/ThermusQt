// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

extern TTMThermalModelBQ *gModelBQConQNetBDens;
extern double gBQyQNetBDens[1];

void BQfuncQNetBDens(int n, double x[], double f[])
{
    (gModelBQConQNetBDens->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
    (gModelBQConQNetBDens->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);

    double y[1];
    y[0]      = gModelBQConQNetBDens->getParameterSet()->getB2Q();

    int check = gModelBQConQNetBDens->primPartDens();

    if (!check) {
        double nb = gModelBQConQNetBDens->getBaryon();
        f[1]      = (nb - gBQyQNetBDens[0]) / gBQyQNetBDens[0];
        f[2]      = (gModelBQConQNetBDens->getBaryon() /2. / gModelBQConQNetBDens->getCharge() - y[0]) / y[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
