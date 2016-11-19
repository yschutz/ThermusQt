// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: November 2016

//__________________________________________________________________________
// Base class of parameter set objects. All derived classes must define
// Double_t GetRadius(), since this is used by this class to calculate
// the volume.
//

//__________________________________________________________________________
#include "TTMParameterSet.h"

TTMParameterSet::TTMParameterSet(QObject *parent) : QObject(parent),
    mPar(NULL), mConstraintInfo("Parameters unconstrained")
{
// ctor
}
