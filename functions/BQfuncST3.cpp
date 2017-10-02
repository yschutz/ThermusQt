// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "functions/FncsConstrain.h"

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>
#include <QtMath>

//__________________________________________________________________________
int BQfuncST3(const gsl_vector* x, void* p, gsl_vector* f)
{
    int rv = 0;
    TTMThermalModelBQ* model = ((PARAMETERS *)p)->p0;
    (model->getParameterSet())->getParameter(TTMParameterSet::kMUB)->setValue(gsl_vector_get(x, 0));

    bool check = model->primPartDens();

    if (check) {
        model->generateEntropyDens();
        double st3 = ((PARAMETERS*)p)->p1;
        gsl_vector_set(f, 0, (model->getEntropy() / qPow(model->getParameterSet()->getT(), 3.) * qPow(0.197,3.) -
                              st3) / st3);
    } else {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Primary particles density problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
