// Author: Spencer Wheaton 14 July 2004 //
//__________________________________________________________________________
// Strangeness-canonical thermal fit class
// Adapted for Qt by Yves Schutz: September 2017

#include "TTMThermalFitBQ.h"

//__________________________________________________________________________
TTMThermalFitBQ::TTMThermalFitBQ(QObject *parent) : TTMThermalFit(parent),
  mNonStrangeQStats(true), mParm(nullptr), mWidth(true)
{
    // ctor

    mDescriptor = "SCanonical";

}
