// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017
#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//void broydn(double x[], int n, int *check,
//            void (*vecfunc)(int, double [], double []));

void BQfuncQST3(int n, double x[], double f[]);

TTMThermalModelBQ *gModelBQConQST3;
double gBQyQST3[1];

int BQConstrainQST3(TTMThermalModelBQ *model, double SoverT3)
{    

    gModelBQConQST3 = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    int  check = 0;
    double *x, *fbroydn;

    //  x=(double*)dvector(1,2);
    //  fbroydn=(double*)dvector(1,2);

    x[1]        = model->getParameterSet()->getMuB();
    x[2]        = model->getParameterSet()->getMuQ();
    gBQyQST3[0] = SoverT3;

    if (gBQyQST3[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot constrain S/T^3 to zero");
        msg.exec();
        return 1;
    } else if(model->getParameterSet()->getB2Q() == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot constrain B/2Q to zero");
        msg.exec();
        return 1;
    } else {
        //    broydn(x,2,&check,BQfuncQST3);
        BQfuncQST3(2, x, fbroydn);
        if (check) {
            QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Convergence problems");
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain S/T^3 and B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);
            model->getParameterSet()->setConstraintInfo("S/T^3 and B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
