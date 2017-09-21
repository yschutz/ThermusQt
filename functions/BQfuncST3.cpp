// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

extern TTMThermalModelBQ *gModelBQConST3;
extern double gBQyST3[1];

void BQfuncST3(int n, double x[], double f[])
{
    (gModelBQConST3->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);

    int check = gModelBQConST3->primPartDens();

    if (!check) {
        gModelBQConST3->generateEntropyDens();
        f[1] = (gModelBQConST3->getEntropy() / qPow(gModelBQConST3->getParameterSet()->getT(), 3.) * qPow(0.197,3.) -
                gBQyST3[0]) / gBQyST3[0];
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Prim part dens problems!");
        msg.exec();
    }
}
