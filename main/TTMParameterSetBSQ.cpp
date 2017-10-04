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
TTMParameterSetBSQ::TTMParameterSetBSQ()
{
    // default ctor
    mB2Q = 0.;

    for (int index = 0; index < kPARTYPES; index++)
        mPar.append(new TTMParameter);

    for (qint32 type = 0; type < kPARTYPES; type++) {
        mPar[type]->setParameter(name((ParameterType)type), 0.0, 0.0);
        mDens[type] = 0.0;
        mConstrain[type] = false;
    }
    mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBSQ::TTMParameterSetBSQ(double temp, double mub, double mus, double muq, double gs, double r,
                                       double muc, double gc, double mubeauty, double gb, double b2q, double s, double c, double beauty,
                                       double temp_error, double mub_error, double mus_error, double muq_error, double gs_error, double r_error,
                                       double muc_error, double gc_error, double mubeauty_error, double gb_error) :
    TTMParameterSet()
{
    // Sets all parameters and their errors as well as B/2Q, S/V, C/V and b/V.
    // All parameters are set as "fixed type".
    //

    mB2Q             = b2q;
    mDens[kMUS]      = s;
    mDens[kMUC]      = c;
    mDens[kMUBEAUTY] = beauty;

    for (int index = 0; index < kPARTYPES; index++)
        mPar.append(new TTMParameter);

    mPar[kT]->setParameter(name(kT), temp, temp_error);

    mPar[kMUB]->setParameter(name(kMUB), mub, mub_error);
    mConstrain[kMUB] = false;

    mPar[kMUS]->setParameter(name(kMUS), mus, mus_error);
    mConstrain[kMUS] = false;

    mPar[kMUQ]->setParameter(name(kMUQ), muq, muq_error);
    mConstrain[kMUQ] = false;

    mPar[kGAMMAS]->setParameter(name(kGAMMAS), gs, gs_error);

    mPar[kRADIUS]->setParameter(name(kRADIUS), r, r_error);

    mPar[kMUC]->setParameter(name(kMUC), muc, muc_error);
    mConstrain[kMUC] = false;

    mPar[kGAMMAC]->setParameter(name(kGAMMAC), gc, gc_error);

    mPar[kMUBEAUTY]->setParameter(name(kMUBEAUTY), mubeauty, mubeauty_error);
    mConstrain[kMUBEAUTY] = false;

    mPar[kGAMMABEAUTY]->setParameter(name(kGAMMABEAUTY), gb, gb_error);

    mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBSQ::TTMParameterSetBSQ(const TTMParameterSetBSQ &set)
{
    // copy ctor
    mB2Q               = set.getB2Q();

    for (int type = 0; type < kPARTYPES; type++) {
        mPar.append(new TTMParameter(set.mPar.at(type)));
        mDens[(ParameterType)type]      = set.get((ParameterType)type);
        mConstrain[(ParameterType)type] = set.getConstrain((ParameterType)type);
    }

    mConstraintInfo    = set.getConstraintInfo();

    setParent(set.parent());
}

//__________________________________________________________________________
void TTMParameterSetBSQ::constrain(ParameterType type, double x)
{
    // Changes parameter type to a constrained type parameter. x is the initial value
    //

    mDens[type]      = x;
    mConstrain[type] = true;
    mPar[type]->constrain();

    mB2Q = mDens[kMUQ];

    mConstraintInfo = "Parameters constrained";
}

//__________________________________________________________________________
void TTMParameterSetBSQ::list()
{
    // list all the parameters

    qInfo() << "  ***************************** Thermal Parameters **************************** ";
    for (qint32 i = 0; i < kPARTYPES; i++) {
        mPar[i]->list();
        if ((ParameterType)i == kMUS && mConstrain[kMUS]) {
            qInfo() << "\t\t\t\t\t\t\t\t S/V:    " << mDens[kMUS];
        } else if ((ParameterType)i == kMUQ && mConstrain[kMUQ]) {
            qInfo() << "\t\t\t\t\t\t\t\t B/2Q: " << mB2Q;
        } else if ((ParameterType)i == kMUC && mConstrain[kMUC]) {
            qInfo() << "\t\t\t\t\t\t\t\t C/V: " << mDens[kMUC];
        } else if ((ParameterType)i == kMUBEAUTY && mConstrain[kMUBEAUTY]) {
            qInfo() << "\t\t\t\t\t\t\t\t Beauty/V: " << mDens[kMUBEAUTY];
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

    for (qint32 type = 0; type < kPARTYPES; type++) {
        mPar.append(new TTMParameter(obj.mPar.at(type)));
        mDens[(ParameterType)type]      = obj.get((ParameterType)type);
        mConstrain[(ParameterType)type] = obj.getConstrain((ParameterType)type);
    }

    mConstraintInfo    = obj.getConstraintInfo();

    setParent((obj.parent()));

    return *this;
}
