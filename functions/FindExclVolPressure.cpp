// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "functions/FncsConstrain.h"

#include "main/TTMThermalModelBSQ.h"

TTMThermalModelBSQ *gModelBSQExcVol;

double FindExclVolPressure(TTMThermalModelBSQ *model, double limit)
{
    PARAMETERSS params;
    params.p0 = model;
    params.p1 = 0.0;
    params.p2 = 0.0;
    params.p3 = 0.0;

    int status;
    return brent(0.0, 1.1 * limit, 1E-4, status, params, ExclVolPressureFunc);
}
