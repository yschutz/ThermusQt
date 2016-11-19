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
TTMParameterSetBSQ::TTMParameterSetBSQ(double temp, double mub, double mus, double muq, double gs, double r,
                                       double muc, double gc, double mubeauty, double gb, double b2q, double s, double c, double beauty,
                                       double temp_error, double mub_error, double mus_error, double muq_error, double gs_error, double r_error,
                                       double muc_error, double gc_error, double mubeauty_error, double gb_error) :
    TTMParameterSet()
{
    // Sets all parameters and their errors as well as B/2Q, S/V, C/V and b/V.
    // All parameters are set as "fixed type".
    //

    mB2Q                      = b2q;
    mDens[ParaSel::kMuS]      = s;
    mDens[ParaSel::kMuC]      = c;
    mDens[ParaSel::kMuBeauty] = beauty;

    mPar = new TTMParameter[ParaSel::kParTypes];

    mPar[ParaSel::kTemp].setParameter(ParaSel::getParaName(ParaSel::kTemp), temp, temp_error);

    mPar[ParaSel::kMuB].setParameter(ParaSel::getParaName(ParaSel::kMuB), mub, mub_error);
    mConstrain[ParaSel::kMuB] = false;

    mPar[ParaSel::kMuS].setParameter(ParaSel::getParaName(ParaSel::kMuS), mus, mus_error);
    mConstrain[ParaSel::kMuS] = false;

    mPar[ParaSel::kMuQ].setParameter(ParaSel::getParaName(ParaSel::kMuQ), muq, muq_error);
    mConstrain[ParaSel::kMuQ] = false;

    mPar[ParaSel::kGammaS].setParameter(ParaSel::getParaName(ParaSel::kGammaS), gs, gs_error);

    mPar[ParaSel::kRadius].setParameter(ParaSel::getParaName(ParaSel::kRadius), r, r_error);

    mPar[ParaSel::kMuC].setParameter(ParaSel::getParaName(ParaSel::kMuC), muc, muc_error);
    mConstrain[ParaSel::kMuC] = false;

    mPar[ParaSel::kGammaC].setParameter(ParaSel::getParaName(ParaSel::kGammaC), gc, gc_error);

    mPar[ParaSel::kMuBeauty].setParameter(ParaSel::getParaName(ParaSel::kMuBeauty), mubeauty, mubeauty_error);
    mConstrain[ParaSel::kMuBeauty] = false;

    mPar[ParaSel::kGammaBeauty].setParameter(ParaSel::getParaName(ParaSel::kGammaBeauty), gb, gb_error);

    mConstraintInfo = "Parameters unconstrained";
}

//__________________________________________________________________________
TTMParameterSetBSQ::TTMParameterSetBSQ()  : mB2Q(0.)

{
    // default ctor

    mPar = new TTMParameter[ParaSel::kParTypes];
    for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
        mPar[type].setParameter(ParaSel::getParaName((ParaSel::ParameterType)type), 0.0, 0.0);
        mDens[type] = 0.0;
        mConstrain[type] = false;
    }
    mConstraintInfo = "Parameters unconstrained";

}

//__________________________________________________________________________
void TTMParameterSetBSQ::constrain(ParaSel::ParameterType type, double x)
{
    // Changes parameter type to a constrained type parameter. x is the initial value
    //

    mDens[type]      = x;
    mConstrain[type] = true;
    mPar[type].constrain();

    mB2Q = mDens[ParaSel::kMuQ];

    mConstraintInfo = "Parameters constrained";
}

//__________________________________________________________________________
void TTMParameterSetBSQ::list()
{
    // list all the parameters

    qInfo() << "  ***************************** Thermal Parameters **************************** ";
    for (qint32 i = 0; i < ParaSel::kParTypes; i++) {
        mPar[i].list();
        if ((ParaSel::ParameterType)i == ParaSel::kMuS && mConstrain[ParaSel::kMuS]) {
            qInfo() << "\t\t\t\t\t\t\t\t S/V:    " << mDens[ParaSel::kMuS];
        } else if ((ParaSel::ParameterType)i == ParaSel::kMuQ && mConstrain[ParaSel::kMuQ]) {
            qInfo() << "\t\t\t\t\t\t\t\t B/2Q: " << mB2Q;
        } else if ((ParaSel::ParameterType)i == ParaSel::kMuC && mConstrain[ParaSel::kMuC]) {
            qInfo() << "\t\t\t\t\t\t\t\t C/V: " << mDens[ParaSel::kMuC];
        } else if ((ParaSel::ParameterType)i == ParaSel::kMuBeauty && mConstrain[ParaSel::kMuBeauty]) {
            qInfo() << "\t\t\t\t\t\t\t\t Beauty/V: " << mDens[ParaSel::kMuBeauty];
        }
    }

    qInfo() << "\t\t\t " << mConstraintInfo;
    qInfo() << "  ******************************************************************************";
}

//__________________________________________________________________________
TTMParameterSetBSQ &TTMParameterSetBSQ::operator=(const TTMParameterSetBSQ &obj)
{
    // assignation operatoe

    if (this == &obj) return *this;

    mB2Q               = obj.getB2Q();

    mPar = new TTMParameter[ParaSel::kParTypes];
    for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
        mPar[(ParaSel::ParameterType)type] = *(obj.getParameter((ParaSel::ParameterType)type));
        mDens[(ParaSel::ParameterType)type] = obj.get((ParaSel::ParameterType)type);
        mConstrain[(ParaSel::ParameterType)type] = obj.getConstrain((ParaSel::ParameterType)type);
        mDens[(ParaSel::ParameterType)type] = obj.getDens((ParaSel::ParameterType)type);
    }

    mConstraintInfo    = obj.getConstraintInfo();

    return *this;
}
