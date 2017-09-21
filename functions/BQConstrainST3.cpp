// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//void broydn(double x[], int n, int *check,
//            void (*vecfunc)(int, double [], double []));

void BQfuncST3(int n, double x[], double f[]);

TTMThermalModelBQ *gModelBQConST3;
double gBQyST3[1];

int BQConstrainST3(TTMThermalModelBQ *model, double soverT3)
{    

    gModelBQConST3 = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    int  check = 0;

    double *x, *fbroydn;

    //  x=(double*)dvector(1,1);
    //  fbroydn=(double*)dvector(1,1);

    x[1]       = model->getParameterSet()->getMuB();
    gBQyST3[0] = soverT3;

    if (gBQyST3[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot conserve S/T^3 to zero");
        msg.exec();
        return 1;
    } else {
        //    broydn(x,1,&check,BQfuncST3);
        BQfuncST3(1, x, fbroydn);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Convergence problems");
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/T^3");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
            model->getParameterSet()->setConstraintInfo("S/T^3 Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
