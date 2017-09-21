// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

extern TTMThermalModelBQ *gModelBQConQST3;
extern double gBQyQST3[1];

void BQfuncQST3(int n, double x[], double f[])
{
    (gModelBQConQST3->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
    (gModelBQConQST3->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);

    double y[1];
    y[0]      = gModelBQConQST3->getParameterSet()->getB2Q();
    int check = gModelBQConQST3->primPartDens();

    if (!check) {
        gModelBQConQST3->generateEntropyDens();
        f[1] = (gModelBQConQST3->getEntropy() / qPow(gModelBQConQST3->getParameterSet()->getT(), 3.) * qPow(0.197, 3.) - gBQyQST3[0]) / gBQyQST3[0];
        f[2] = (gModelBQConQST3->getBaryon() / 2. / gModelBQConQST3->getCharge() - y[0])/y[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
