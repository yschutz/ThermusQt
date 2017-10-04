// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: September 2017
//__________________________________________________________________________
// The parameter set to be used when treating B and Q grand-canonically and
// S canonically. B/2Q may be used to constrain muQ when this parameter
// set is linked to a particle set in a ThermalModelBQ object

#include "TTMParameterSetBQ.h"

//__________________________________________________________________________
TTMParameterSetBQ::TTMParameterSetBQ()
{
    // ctor
    // Sets parameter names and values and errors to 0.
    //

      mB2Q = 0.;
      mS   = 0.;

      for (int index = 0; index <= kRADIUS; index++)
          mPar.append(new TTMParameter);

      mPar[kT]->setParameter("T", 0., 0.);
      mPar[kMUB]->setParameter("muB", 0., 0.);
      mPar[kMUQ]->setParameter("muQ", 0., 0.);
      mMuQConstrain = false;
      mPar[kGAMMAS]->setParameter("gammas", 0., 0.);
      mPar[kCRADIUS]->setParameter("Can. radius", 0., 0.);
      mCorrRConstrain = false;
      mPar[kRADIUS]->setParameter("radius", 0., 0.);
      mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBQ::TTMParameterSetBQ(double temp, double mub, double muq, double gs, double can_r,
                                     double r, double b2q, double strangeness, double temp_error,
                                     double mub_error, double muq_error, double gs_error,
                                     double can_r_error, double r_error)
{
    // Sets all parameters and their errors as well as B/2Q.
    // All parameters are set as "fixed type".
    //

    mB2Q = b2q;
    mS   = strangeness;

    for (int index = 0; index <= kRADIUS; index++)
        mPar.append(new TTMParameter);

    mPar[kT]->setParameter("T", temp, temp_error);
    mPar[kMUB]->setParameter("muB", mub, mub_error);
    mPar[kMUQ]->setParameter("muQ", muq, muq_error);
    mMuQConstrain = false;
    mPar[kGAMMAS]->setParameter("gammas", gs, gs_error);
    mPar[kCRADIUS]->setParameter("Can. radius", can_r, can_r_error);
    mCorrRConstrain = false;
    mPar[kRADIUS]->setParameter("radius", r, r_error);
    mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBQ::TTMParameterSetBQ(const TTMParameterSetBQ &set)
{
    // copy ctor
    mB2Q                = set.getB2Q();
    mS                  = set.getS();
    mConstraintInfo     = set.getConstraintInfo();
    mMuQConstrain       = set.getMuQConstrain();
    mCorrRConstrain     = set.getCorrRConstrain();
    mPar[kT]            = set.getParameter(kT);
    mPar[kMUB]          = set.getParameter(kMUB);
    mPar[kMUQ]          = set.getParameter(kMUQ);
    mPar[kGAMMAS]       = set.getParameter(kGAMMAS);
    mPar[kCRADIUS]      = set.getParameter(kCRADIUS);
    mPar[kRADIUS]       = set.getParameter(kRADIUS);

    setParent(set.parent());
}

//__________________________________________________________________________
void TTMParameterSetBQ::conserveSGlobally()
{
    // Changes the correlation radius to a constrained type parameter.

    mCorrRConstrain = true;
    mPar[kCRADIUS]->constrain();
}

//__________________________________________________________________________
void TTMParameterSetBQ::constrainMuQ(double b2q)
{
    // Changes muQ to a constrained type parameter. x is the initial B/2Q
     // ratio in the system.

     mB2Q = b2q;
     mMuQConstrain = true;
     mPar[kMUQ]->constrain();
}

//__________________________________________________________________________
TTMParameterSetBQ &TTMParameterSetBQ::operator=(const TTMParameterSetBQ &obj)
{
    // assign operator
    if (this == &obj) return *this;

    mB2Q                = obj.getB2Q();
    mS                  = obj.getS();
    mConstraintInfo     = obj.getConstraintInfo();
    mMuQConstrain       = obj.getMuQConstrain();
    mCorrRConstrain     = obj.getCorrRConstrain();
    mPar[kT]            = obj.getParameter(kT);
    mPar[kMUB]          = obj.getParameter(kMUB);
    mPar[kMUQ]          = obj.getParameter(kMUQ);
    mPar[kGAMMAS]       = obj.getParameter(kGAMMAS);
    mPar[kCRADIUS]      = obj.getParameter(kCRADIUS);
    mPar[kRADIUS]       = obj.getParameter(kRADIUS);

    setParent(obj.parent());

    return *this;
}
