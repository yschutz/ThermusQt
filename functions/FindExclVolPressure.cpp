// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: October 2017

#include "main/TTMThermalModelBSQ.h"

Float_t zbrent(Float_t (*func)(Float_t), Float_t x1, Float_t x2, Float_t tol);

Float_t ExclVolPressureFunc(Float_t x);

TTMThermalModelBSQ *gModelBSQExcVol;

Double_t FindExclVolPressure(TTMThermalModelBSQ *model, Double_t limit)
{

  gModelBSQExcVol = model;

  return zbrent(ExclVolPressureFunc,0.,1.1*limit,1e-4);

}
