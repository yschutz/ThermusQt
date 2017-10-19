// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: 27 Octobre 2016
//__________________________________________________________________________
// The parameter set to be used when treating B, S, Q, C and b grand-canonically.
// S/V, B/2Q, C/V and b/V may be used to constrain muS, muQ, muC and mub respectively
// when this parameter set is linked to a particle set in a TThermalModelBSQ
// object.
//

#include "TTMParameterSetBSQ.h"

#include <QDebug>

//__________________________________________________________________________
TTMParameterSetBSQ::TTMParameterSetBSQ(QObject *parent) : TTMParameterSet(parent),
    mBeautyDens(0.0), mCDens(0.0), mSDens(0.0),
    mMuBeautyConstrain(false), mMuCConstrain(false), mMuQConstrain(false), mMuSConstrain(false)
{
    // default ctor
    mB2Q = 0.;

    for (int index = 0; index < kPARTYPES; index++)
        mPar.append(new TTMParameter);

    for (int type = 0; type < kPARTYPES; type++)
        mPar[type]->setParameter(name((ParameterType)type), 0.0, 0.0);

    mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBSQ::TTMParameterSetBSQ(double temp, double mub, double mus, double muq, double gs, double r,
                                       double muc, double gc, double mubeauty, double gb, double b2q, double s, double c, double beauty,
                                       double temp_error, double mub_error, double mus_error, double muq_error, double gs_error, double r_error,
                                       double muc_error, double gc_error, double mubeauty_error, double gb_error) :
    mBeautyDens(beauty), mCDens(c), mSDens(s),
    mMuBeautyConstrain(false), mMuCConstrain(false), mMuQConstrain(false), mMuSConstrain(false)
{
    // Sets all parameters and their errors as well as B/2Q, S/V, C/V and b/V.
    // All parameters are set as "fixed type".
    //

    mB2Q = b2q;

    for (int index = 0; index < kPARTYPES; index++)
        mPar.append(new TTMParameter);

    mPar[kT]          ->setParameter(name(kT),           temp,     temp_error    );
    mPar[kMUB]        ->setParameter(name(kMUB),         mub,      mub_error     );
    mPar[kMUS]        ->setParameter(name(kMUS),         mus,      mus_error     );
    mPar[kMUQ]        ->setParameter(name(kMUQ),         muq,      muq_error     );
    mPar[kGAMMAS]     ->setParameter(name(kGAMMAS),      gs,       gs_error      );
    mPar[kRADIUS]     ->setParameter(name(kRADIUS),      r,        r_error       );
    mPar[kCRADIUS]    ->setParameter(name(kCRADIUS),     0.,       0.            );
    mPar[kMUC]        ->setParameter(name(kMUC),         muc,      muc_error     );
    mPar[kGAMMAC]     ->setParameter(name(kGAMMAC),      gc,       gc_error      );
    mPar[kMUBEAUTY]   ->setParameter(name(kMUBEAUTY),    mubeauty, mubeauty_error);
    mPar[kGAMMABEAUTY]->setParameter(name(kGAMMABEAUTY), gb,       gb_error      );

    mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBSQ::TTMParameterSetBSQ(const TTMParameterSetBSQ &set)
{
    // copy ctor
    mB2Q               = set.getB2Q();

    for (int type = 0; type < kPARTYPES; type++)
        mPar.append(new TTMParameter(set.mPar.at(type)));
    mBeautyDens        = set.mBeautyDens;
    mCDens             = set.mCDens;
    mSDens             = set.mSDens;
    mMuBeautyConstrain = set.mBeautyDens;
    mMuCConstrain      = set.mMuCConstrain;
    mMuQConstrain      = set.mMuQConstrain;
    mMuSConstrain      = set.mMuSConstrain;
    mConstraintInfo    = set.getConstraintInfo();

    setParent(set.parent());
}

//__________________________________________________________________________
void TTMParameterSetBSQ::constrain(ParameterType type, double x)
{
    // Changes parameter type to a constrained type parameter. x is the initial value
    //
    switch (type) {
    case kMUQ:
        mB2Q               = x;
        mMuQConstrain      = true;
        break;
    case kMUS:
        mSDens             = x;
        mMuSConstrain      = true;
        break;
    case kMUC:
        mCDens             = x;
        mMuCConstrain      = true;
        break;
    case kMUBEAUTY:
        mBeautyDens        = x;
        mMuBeautyConstrain = true;
        break;
    default:
        break;
    }
    mPar[type]->constrain();
    mConstraintInfo = "Parameters constrained";
}

//__________________________________________________________________________
bool TTMParameterSetBSQ::getConstrain(TTMParameterSet::ParameterType type) const
{
    bool rv;
    switch (type) {
    case kMUQ:
        rv = mMuQConstrain;
        break;
    case kMUS:
        rv = mMuSConstrain;
        break;
    case kMUC:
        rv = mMuCConstrain;
        break;
    case kMUBEAUTY:
        rv = mMuBeautyConstrain;
        break;
    default:
        rv = false;
        break;
    }
    return rv;
}

//__________________________________________________________________________
double TTMParameterSetBSQ::getDens(TTMParameterSet::ParameterType type) const
{
    double rv = 0.0;
    switch (type) {
    case kMUQ:
        rv = mB2Q;
        break;
    case kMUS:
        rv = mSDens;
        break;
    case kMUC:
        rv = mCDens;
        break;
    case kMUBEAUTY:
        rv = mBeautyDens;
        break;
    default:
        rv = 0.0;
        break;
    }
    return rv;
}

//__________________________________________________________________________
void TTMParameterSetBSQ::list()
{
    // list all the parameters

    qInfo() << "  ***************************** Thermal Parameters **************************** ";
    for (int i = 0; i < kPARTYPES; i++) {
        mPar[i]->list();
        if ((ParameterType)i == kMUS && mMuSConstrain) {
            qInfo() << "\t\t\t\t\t\t\t\t S/V:    " << mSDens;
        } else if ((ParameterType)i == kMUQ && mMuQConstrain) {
            qInfo() << "\t\t\t\t\t\t\t\t B/2Q: " << mB2Q;
        } else if ((ParameterType)i == kMUC && mMuCConstrain) {
            qInfo() << "\t\t\t\t\t\t\t\t C/V: " << mCDens;
        } else if ((ParameterType)i == kMUBEAUTY && mMuBeautyConstrain) {
            qInfo() << "\t\t\t\t\t\t\t\t Beauty/V: " << mBeautyDens;
        }
    }
    qInfo() << "\t\t\t " << mConstraintInfo;
    qInfo() << "  ******************************************************************************";
}

//__________________________________________________________________________
TTMParameterSetBSQ &TTMParameterSetBSQ::operator=(const TTMParameterSetBSQ &obj)
{
    // assignation operator

    if (this == &obj) return *this;

    mB2Q               = obj.getB2Q();

    for (int type = 0; type < kPARTYPES; type++)
        mPar.append(new TTMParameter(obj.mPar.at(type)));
    mBeautyDens        = obj.mBeautyDens;
    mCDens             = obj.mCDens;
    mSDens             = obj.mSDens;
    mMuBeautyConstrain = obj.mBeautyDens;
    mMuCConstrain      = obj.mMuCConstrain;
    mMuQConstrain      = obj.mMuQConstrain;
    mMuSConstrain      = obj.mMuSConstrain;
    mConstraintInfo    = obj.getConstraintInfo();
    mConstraintInfo    = obj.getConstraintInfo();

    setParent((obj.parent()));

    return *this;
}
