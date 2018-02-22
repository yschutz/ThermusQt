// Author: Spencer Wheaton 26 April 2010
// Adapted for Qt by Yves Schutz: 24 Octobre 2016

// Stores particle properties relevant to the thermal model.
// fName is determined from the Monte Carlo particle ID using the function
// Int_2_String(int x). fName is the key used to find objects in ROOT hash
// tables (as in the TTMParticleSet class). It is important that the
// conversion from ID to fName is consistent with that in TTMParticleSet.
//

#include <QDebug>
#include <QFile>

#include "TTMParticle.h"

//__________________________________________________________________________
TTMParticle::TTMParticle(QObject* parent) : QObject(parent),
    mName(""), mPDG(0), mSpin(-1), mStatistic(-1), mMass(-1.0),
    mS(-123), mBaryon(-123), mCharge(-123), mC(-123), mB(-123), mT(-123),
    mSC(-123), mCC(-123), mBC(-123) , mTC(-123),
    mWidth(-123.), mLifetime(-123.), mRadius(-1.), mThreshold(-123.0), mNdecay(-1)
{
    // default ctor
}

//__________________________________________________________________________
TTMParticle::TTMParticle(QString name, int pdg, int spin, int statistic, double mass, int strange, int baryon, int charge, int charm, int beauty, int top, int sContent, int cContent, int bContent, int tContent, double width, double lifetime, double radius, double threshold, int ndecay, QObject *parent) :
    QObject(parent),
    mName(name), mPDG(pdg), mSpin(spin), mStatistic(statistic), mMass(mass),
    mS(strange), mBaryon(baryon), mCharge(charge), mC(charm), mB(beauty), mT(top),
    mSC(sContent), mCC(cContent), mBC(bContent) , mTC(tContent),
    mWidth(width), mLifetime(lifetime), mRadius(radius), mThreshold(threshold), mNdecay(ndecay)
{
    //ctor
}

//__________________________________________________________________________
TTMParticle::TTMParticle(TTMParticle& obj, QObject* parent) : QObject(parent)
{
    //cpy ctor
//    setID(obj.getID());
    mName      = obj.mName;
    mPDG       = obj.mPDG;
    mSpin      = obj.mSpin;
    mStatistic = obj.mStatistic;
    mMass      = obj.mMass;
    mS         = obj.mS;
    mBaryon    = obj.mBaryon;
    mCharge    = obj.mCharge;
    mC         = obj.mC;
    mB         = obj.mB;
    mT         = obj.mT;
    mSC        = obj.mSC;
    mCC        = obj.mCC;
    mBC        = obj.mBC;
    mTC        = obj.mTC;
    mWidth     = obj.mWidth;
    mLifetime  = obj.mLifetime;
    mRadius    = obj.mRadius;
    mThreshold = obj.mThreshold;
    mNdecay    = obj.mNdecay;
}

//__________________________________________________________________________
void TTMParticle::list() const
{
    qDebug() << Q_FUNC_INFO << mName <<
        mPDG << mSpin <<  mStatistic << mMass  << mS << mBaryon << mCharge <<
        mC << mB << mT << mSC << mCC << mBC << mTC <<
        mWidth << mLifetime << mRadius << mThreshold << mNdecay;
}
