// Author: Spencer Wheaton 1 July 2004 //
// Adapted for Qt by Yves Schutz: Octobre 2016

//__________________________________________________________________________
// Stores daughter id's as well as the branching ratio as a
// fraction
//
#include <QDebug>

#include "TTMDecayChannel.h"

//__________________________________________________________________________
TTMDecayChannel::TTMDecayChannel(QObject* parent) : QObject(parent),
    mBRatio(0.0), mDaughters(NULL)
{
    //ctor
}

//__________________________________________________________________________
TTMDecayChannel::TTMDecayChannel(double fraction, QList<TTMIDObj *> *list, QObject* parent) :
    QObject(parent),
    mBRatio(fraction), mDaughters(list)
{
    //ctor

}

//__________________________________________________________________________
void TTMDecayChannel::list()
{
    //list all daughters

    qDebug() << Q_FUNC_INFO << "INFO: BRatio: " << mBRatio;
    if (mDaughters) {
        QList<TTMIDObj*>::iterator i;
        for (i = mDaughters->begin(); i != mDaughters->end(); ++i)
            qDebug() << Q_FUNC_INFO << "INFO: " << (*i)->getID() << endl;
    } else {
        qWarning("%s WARNING: No daughters list defined", Q_FUNC_INFO);
    }
}

//__________________________________________________________________________
TTMDecayChannel &TTMDecayChannel::operator=(const TTMDecayChannel &obj)
{
    // assignation operator
    if (this != &obj) {
        mBRatio    = obj.getBRatio();
        mDaughters = obj.getDaughterList();
    }
    return *this;
}
