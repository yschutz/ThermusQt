// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt: Yves Schutz Septembre 2017

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalModelBQ.h"

extern TTMThermalModelBQ *gModelBQConQ;
extern int gCheck;

void BQfuncQ(int n, double x[], double f[])
{
  (gModelBQConQ->getParameterSet())->getParameter(TTMParameterSet::kMUQ)->setValue(x[1]);
  gCheck = gModelBQConQ->primPartDens();

  double y[1];
  y[0] = gModelBQConQ->getParameterSet()->getB2Q();
  f[1] = (gModelBQConQ->getBaryon() / 2. / gModelBQConQ->getCharge() - y[0]) / y[0];
}
