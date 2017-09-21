// Author: Spencer Wheaton 14 July 2004 //
// Adapted for QT: Yves Schutz September 2017

#include "main/TTMParameterSet.h"
#include "main/TTMThermalModelBQ.h"
#include <QMessageBox>
#include <QtMath>


//void broydn(double x[], int n, int *check,
//            void (*vecfunc)(int, double [], double []));

void BQfuncQEN(int n, double x[], double f[]);

TTMThermalModelBQ *gModelBQConQEN;
double gBQyQEN[1];

int BQConstrainQEN(TTMThermalModelBQ *model, double eovern)
{    
    gModelBQConQEN = model;
    model->getParameterSet()->getParameter(1)-setStatus("");
    model->getParameterSet()->getParameter(2)->setStatus("");
    int  check=0;
    double *x, *fbroydn;

    //  x=(double*)dvector(1,2);
    //  fbroydn=(double*)dvector(1,2);

    x[1]       = model->getParameterSet()->getMuB();
    x[2]       = model->getParameterSet()->getMuQ();
    gBQyQEN[0] = eovern;

    if (gBQyQEN[0] == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot constrain E/N to zero");
        msg.exec();
        return 1;
    } else if (model->getParameterSet()->getB2Q() == 0.) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, "Cannot constrain B/2Q to zero");
        return 1;
    } else {
        //    broydn(x,2,&check,BQfuncQEN);
        BQfuncQEN(2, x, fbroydn);
        if (check) {
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "Convergence problems");
            model->getParameterSet()->setConstraintInfo("Unable to Constrain E/N and B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUB)->setValue(0.);
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(1)->SetValue(x[1]);
            model->getParameterSet()->getParameter(2)->SetValue(x[2]);
            model->getParameterSet()->setConstraintInfo("E/N and B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(1)->setStatus("(*CONSTRAINED*)");
            model->getParameterSet()->getParameter(2)->setStatus("(*CONSTRAINED*)");

            return 0;
        }
    }
}
