// Author: Spencer Wheaton 14 July 2004 //
// Adaptated for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

#include <QMessageBox>

//void broydn(double x[], int n, int *check,
//            void (*vecfunc)(int, double [], double []));

void BQfuncQPercolation(int n, double x[], double f[]);

TTMThermalModelBQ *gModelBQConQPercolation;

int BQConstrainQPercolation(TTMThermalModelBQ *model)
{    

    gModelBQConQPercolation = model;
    model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("");
    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    int  check = 0;
    double *x, *fbroydn;

    //  x=(double*)dvector(1,2);
    //  fbroydn=(double*)dvector(1,2);

    x[1] = model->getParameterSet()->getMuB();
    x[2] = model->getParameterSet()->getMuQ();

    //    broydn(x,2,&check,BQfuncQPercolation);
    BQfuncQPercolation(2, x, fbroydn);
    if (check) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Convergence problems");
        msg.exec();
        model->getParameterSet()->setConstraintInfo("Unable to Constrain to Percolation Model and B/2Q");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
        model->GetParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
        model->GetParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
        return 1;
    } else {
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(x[1]);
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(x[2]);
        model->getParameterSet()->setConstraintInfo("Percolation Model and B/2Q Successfully Constrained");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(*CONSTRAINED*)");
        model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");

        return 0;
    }
}
