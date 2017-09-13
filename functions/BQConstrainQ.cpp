// Author: Spencer Wheaton 14 July 2004 //

#include "main/TTMParameterSet.h"
#include "main/TTMThermalmodelBQ.h"
//#include <nrutil.h>

#include <QMessageBox>
#include <QObject>

//void broydn(float x[], int n, int *check, void (*vecfunc)(int, float [], float []));
//void BQfuncQ(int n, float x[], float f[]);

TTMThermalModelBQ *gModelBQConQ;
int gCheck;

int BQConstrainQ(TTMThermalModelBQ *model)
{

    gModelBQConQ = model;
    gCheck       = 0;

    model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("");
    int  check = 0;
    float *x, *fbroydn;
    double y[1];


    //  x=(float*)dvector(1,1);
    //  fbroydn=(float*)dvector(1,1);

    x[1] = model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->getValue();
    y[0] = model->getParameterSet()->getB2Q();

    if(y[0] == 0.){
        QMessageBox msg(QMessageBox::Critical, "BQConstrainQ", "Cannot constrain B/2Q to zero");
        msg.exec();
        return 1;
    } else {
        //    broydn(x,1,&check,BQfuncQ);
        //    BQfuncQ(1,x,fbroydn);

        if(gCheck) {
            QMessageBox msg(QMessageBox::Critical, "BQConstrainQ", "Problems with parameters");
            msg.exec();
            check = 1;
        }

        if(check) {
            QMessageBox msg(QMessageBox::Critical, "BQConstrainQ", "Convergence problems");
            msg.exec();
            model->getParameterSet()->setConstraintInfo("Unable to Constrain B/2Q");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(Unable to constrain)");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(0.);
            return 1;
        } else {
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setValue(x[1]);
            model->getParameterSet()->setConstraintInfo("B/2Q Successfully Constrained");
            model->getParameterSet()->getParameter(TTMParameterSet::kMUQ)->setStatus("(*CONSTRAINED*)");
            return 0;
        }
    }
}
