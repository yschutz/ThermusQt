// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//void broydn(double x[], int n, int *check,
//            void (*vecfunc)(int, double [], double []));

void BQfuncBDens(int n, double x[], double f[]);

TTMThermalModelBQ *gModelBQConBDens;
double gBQyBDens[1];

int BQConstrainBDens(TTMThermalModelBQ *model, double nb)
{    

    gModelBQConBDens = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    int  check = 0;
    double *x, *fbroydn;

    //  x=(double*)dvector(1,1);
    //  fbroydn=(double*)dvector(1,1);

    x[1]         = model->getParameterSet()->getMuB();
    gBQyBDens[0] = nb;

    if (gBQyBDens[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot conserve nb to zero");
        msg.exec();
        return 1;
    } else {
        //    broydn(x,1,&check,BQfuncBDens);
        BQfuncBDens(1, x, fbroydn);
        if(check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Convergence problems");
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain nb");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
            model->getParameterSet()->setConstraintInfo("nb Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
