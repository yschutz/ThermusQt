// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Octobre 2016

//__________________________________________________________________________
// Stores parent & daughter id's as well as the branching ratio as a
// fraction
//

#include <QDebug>

#include "TTMDecay.h"

//__________________________________________________________________________
TTMDecay::TTMDecay(QObject* parent) : QObject(parent),
    mParentID(0), mDaughterID(0), mBRatio(0.0)
{
    // ctor
}

//__________________________________________________________________________
TTMDecay::TTMDecay(const TTMDecay &decay) : QObject(decay.parent())
{
    // copy ctor
    mParentID   = decay.mParentID;
    mDaughterID = decay.mDaughterID;
    mBRatio     = decay.mBRatio;
}

//__________________________________________________________________________
TTMDecay::TTMDecay(qint32 mother, qint32 daughter, double fraction, QObject* parent) :
    QObject(parent),
    mParentID(mother), mDaughterID(daughter), mBRatio(fraction)
{
    //ctor
}

//__________________________________________________________________________
void TTMDecay::list()
{
    // list data members
    QString text = QString("        BRatio   = %1\n       Parent  = %2\n        Daughter = %3").arg(mBRatio).arg(mParentID).arg(mDaughterID);
    qInfo() << text;
}

//__________________________________________________________________________
TTMDecay &TTMDecay::operator=(const TTMDecay &obj)
{
    // assignation operator

    if (this != &obj) {
        mParentID   = obj.mParentID;
        mDaughterID = obj.mDaughterID;
        mBRatio     = obj.mBRatio;
    }
    return *this;
}
