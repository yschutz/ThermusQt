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
      QMessageBox
    cout<<"Cannot constrain S/T^3 to zero"<<endl;
    return 1;
  }else if(model->GetParameterSet()->GetB2Q() == 0.){
    cout<<"Cannot constrain B/2Q to zero"<<endl;
    return 1;
  }else{
    broydn(x,2,&check,BQfuncQST3);
    BQfuncQST3(2,x,fbroydn);
    if(check)
      { 
        cout<<"Convergence problems"<<endl;
	    
        model->GetParameterSet()->SetConstraintInfo("Unable to Constrain S/T^3 and B/2Q");
        model->GetParameterSet()->GetParameter(1)->SetStatus("(Unable to constrain)");
        model->GetParameterSet()->GetParameter(2)->SetStatus("(Unable to constrain)");

        model->GetParameterSet()->GetParameter(1)->SetValue(0.);
        model->GetParameterSet()->GetParameter(2)->SetValue(0.);
        return 1;
      }
    else
      {
        model->GetParameterSet()->GetParameter(1)->SetValue(x[1]);
        model->GetParameterSet()->GetParameter(2)->SetValue(x[2]);

        model->GetParameterSet()->SetConstraintInfo("S/T^3 and B/2Q Successfully Constrained");
	
        model->GetParameterSet()->GetParameter(1)->SetStatus("(*CONSTRAINED*)");
        model->GetParameterSet()->GetParameter(2)->SetStatus("(*CONSTRAINED*)");

        return 0;
      }
  }
}
