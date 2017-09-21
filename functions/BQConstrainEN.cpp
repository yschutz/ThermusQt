// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz September 2017

#include "main/TTMParameterSet.h"
#include "main/TTMThermalModelBQ.h"
//#include <nrutil.h>

#include <QMessageBox>
#include <QObject>
#include <QtMath>


//void broydn(double x[], int n, int *check,
//            void (*vecfunc)(int, double [], double []));

void BQfuncEN(int n, double x[], double f[]);

TTMThermalModelBQ *gModelBQConEN;
double gBQyEN[1];

//__________________________________________________________________________
int BQConstrainEN(TTMThermalModelBQ *model, double eovern)
{    
    gModelBQConEN = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    int  check = 0;
    double *x, *fbroydn;

    //  x       = (double*)dvector(1,1);
    //  fbroydn = (double*)dvector(1,1);

    x[1] = model->getParameterSet()->getMuB();

    gBQyEN[0] = eovern;

    if (gBQyEN[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot conserve E/N to zero");
        msg.exec();
        return 1;
    } else {
//        broydn(x, 1, &check, BQfuncEN);
        BQfuncEN(1 ,x, fbroydn);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Convergence problems");
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain E/N");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
            model->getParameterSet()->setConstraintInfo("E/N Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
