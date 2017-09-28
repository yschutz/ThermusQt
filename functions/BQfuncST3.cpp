// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

extern TTMThermalModelBQ *gModelBQConST3;
extern double gBQyST3[1];

//__________________________________________________________________________
int BQfuncST3(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConST3->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));

    bool check = gModelBQConST3->primPartDens();

    if (check) {
        gModelBQConST3->generateEntropyDens();
        gsl_vector_set(f, 0, (gModelBQConST3->getEntropy() / qPow(gModelBQConST3->getParameterSet()->getT(), 3.) * qPow(0.197,3.) -
                              gBQyST3[0]) / gBQyST3[0]);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
