// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "FncsConstrain.h"

#include "TTMThermalModelBSQ.h"

//__________________________________________________________________________
double ExclVolPressureFunc(double x, void* p)
{
    TTMThermalModelBSQ* model = ((PARAMETERSS*)p)->p0;

    double pid = model->exclVolShiftedPressure(x);
    return (pid - x) / pid;

}
