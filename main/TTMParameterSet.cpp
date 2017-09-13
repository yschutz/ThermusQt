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
    mConstraintInfo("Parameters unconstrained")
{
    // ctor
}

//__________________________________________________________________________
QString TTMParameterSet::name(TTMParameterSet::ParameterType type)
{
    // names the parameters
    // returns the given name of the parameter

       QString rv = "";
       switch (type) {
       case kT:
           rv = "Temperature";
           break;
       case kMUB:
           rv = "muB";
           break;
       case kMUQ:
           rv = "muQ";
           break;
       case kGAMMAS:
           rv= "gammaS";
           break;
       case kCRADIUS:
           rv = "c-radius";
           break;
       case kRADIUS:
           rv = "radius";
           break;
       case kGAMMAC:
           rv = "gammaC";
           break;
       case kGAMMABEAUTY:
           rv = "gammaBeauty";
           break;
       case kMUS:
           rv = "muS";
           break;
       case kMUC:
           rv = "muC";
           break;
       case kMUBEAUTY:
           rv = "muBeauty";
           break;
       default:
           break;
       }

       return rv;
}
