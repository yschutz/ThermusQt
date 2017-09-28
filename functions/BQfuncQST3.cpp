// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include <gsl/gsl_vector.h>

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

extern TTMThermalModelBQ *gModelBQConQST3;
extern double gBQyQST3[1];

//__________________________________________________________________________
int BQfuncQST3(const gsl_vector *x, void */*p*/, gsl_vector *f)
{
    int rv = 0;
    (gModelBQConQST3->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));
    (gModelBQConQST3->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(gsl_vector_get(x, 1));

    double y = gModelBQConQST3->getParameterSet()->getB2Q();
    bool check = gModelBQConQST3->primPartDens();

    if (check) {
        gModelBQConQST3->generateEntropyDens();
        gsl_vector_set(f, 0, (gModelBQConQST3->getEntropy() / qPow(gModelBQConQST3->getParameterSet()->getT(), 3.) * qPow(0.197, 3.) - gBQyQST3[0]) / gBQyQST3[0]);
        gsl_vector_set(f, 1, (gModelBQConQST3->getBaryon() / 2. / gModelBQConQST3->getCharge() - y)/y);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
